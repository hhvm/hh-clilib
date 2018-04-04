<?hh // strict
/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

use namespace Facebook\TypeAssert;
use namespace HH\Lib\{C, Dict, Str, Vec};

<<__ConsistentConstruct>>
abstract class CLIBase {
  private vec<string> $arguments = vec[];

  abstract protected function getSupportedOptions(
  ): vec<CLIOptions\CLIOption>;

  abstract public function mainAsync(): Awaitable<int>;

  final public function getArgv(): vec<string> {
    return $this->argv;
  }

  final protected function getArguments(): vec<string> {
    invariant(
      ($_ = $this) instanceof CLIWithArguments,
      "Calling getArguments(), but don't accept arguments",
    );
    return $this->arguments;
  }

  protected function supportsColors(): bool {
    static $cache;

    if ($cache !== null) {
      return $cache;
    }

    $colors = \getenv('CLICOLORS');
    if (
      $colors === '1'
      || $colors === 'yes'
      || $colors === 'true'
      || $colors === 'on'
    ) {
      $cache = true;
      return true;
    }
    if (
      $colors === '0'
      || $colors === 'no'
      || $colors === 'false'
      || $colors === 'off'
    ) {
      $cache = false;
      return false;
    }

    if (\getenv('TRAVIS') === 'true') {
      $cache = true;
      return true;
    }

    if (\getenv('CIRCLECI') === 'true') {
      $cache = true;
      return true;
    }

    if ($this->isInteractive()) {
      $cache = Str\contains((string) \getenv('TERM'), 'color');
      return $cache;
    }

    $cache = false;
    return false;
  }

  protected function isInteractive(): bool {
    static $cache = null;
    if ($cache !== null) {
      return $cache;
    }

    // Explicit override
    $noninteractive = \getenv('NONINTERACTIVE');
    if ($noninteractive !== false) {
      if ($noninteractive === '1' || $noninteractive === 'true') {
        $cache = false;
        return false;
      }
      if ($noninteractive === '0' || $noninteractive === 'false') {
        $cache = true;
        return true;
      }
      $this->stderr->write(
        "NONINTERACTIVE env var must be 0/1/yes/no/true/false, or unset.\n",
      );
    }

    // Detects TravisCI and CircleCI; Travis gives you a TTY for STDIN
    $ci = \getenv('CI');
    if ($ci === '1' || $ci === 'true') {
      $cache = false;
      return false;
    }

    // Generic
    if (\posix_isatty(\STDIN) && \posix_isatty(\STDOUT)) {
      $cache = true;
      return true;
    }

    // Fail-safe
    $cache = false;
    return false;
  }

  final protected function getStdout(): OutputInterface {
    return $this->stdout;
  }

  final protected function getStderr(): OutputInterface {
    return $this->stderr;
  }

  final public static function main(): noreturn {
    try {
      $responder = new static(
        vec(/* HH_IGNORE_ERROR[2050] */ $GLOBALS['argv']),
        new FileHandleOutput(\STDOUT),
        new FileHandleOutput(\STDERR),
      );
      $exit_code = \HH\Asio\join($responder->mainAsync());
      exit($exit_code);
    } catch (ExitException $e) {
      exit($e->getCode());
    } catch (CLIException $e) {
      \fwrite(\STDERR, $e->getMessage()."\n");
      exit(1);
    }
  }

  public function __construct(
    private vec<string> $argv,
    private OutputInterface $stdout,
    private OutputInterface $stderr,
  ) {
    // Ignore process name in $argv[0]
    $argv = Vec\drop($argv, 1);

    if (C\contains($argv, '--help') || C\contains($argv, '-h')) {
      $this->displayHelp($stdout);
      throw new ExitException(0);
    }

    $not_options = vec[];

    $options = $this->getSupportedOptions();

    $long_options = Dict\pull(
      $options,
      $opt ==> $opt,
      $opt ==> $opt->getLong(),
    );
    $short_options = Vec\filter(
      $options,
      $opt ==> $opt->getShort() !== null,
    ) |> Dict\pull(
      $$,
      $opt ==> $opt,
      $opt ==> (string) $opt->getShort(),
    );

    $arguments = vec[];

    while(!C\is_empty($argv)) {
      $arg = C\firstx($argv);
      $argv = Vec\drop($argv, 1);

      // standard 'stop parsing options here' marker
      if ($arg === '--') {
        break;
      }

      if (Str\starts_with($arg, '--')) {
        $opt = Str\strip_prefix($arg, '--');
        $opts = $long_options;
      } else if (Str\starts_with($arg, '-')) {
        $opt = Str\strip_prefix($arg, '-');
        $opts = $short_options;
      } else {
        $arguments[] = $arg;
        if ((bool) \getenv('POSIXLY_CORRECT')) {
          break;
        }
        continue;
      }

      $value = null;
      if (Str\contains($opt, '=')) {
        $parts = \explode('=', $opt);
        $opt = $parts[0];
        $value = \implode('=', Vec\drop($parts, 1));
      }

      if (!C\contains_key($opts, $opt)) {
        throw new InvalidArgumentException(
          "Unrecognized option: %s",
          $arg,
        );
      }
      $opt = $opts[$opt];
      $argv = $opt->apply($arg, $value, $argv);
    }

    $arguments = Vec\concat($arguments, $argv);
    if (C\is_empty($arguments)) {
      return;
    }
    if (($_ = $this) instanceof CLIWithArguments) {
      $this->arguments = $arguments;
      return;
    }
    throw new InvalidArgumentException(
      "Non-option arguments are not supported; first argument is '%s'",
      C\firstx($arguments),
    );
  }

  public function displayHelp(OutputInterface $out): void {
    $usage = 'Usage: '.C\firstx($this->argv);

    $opts = $this->getSupportedOptions();
    if (C\is_empty($opts)) {
      $usage .= ' [-h|--help]';
    } else {
      $usage .= ' [OPTIONS]';
    }
    if ($this instanceof CLIWithRequiredArguments) {
      $class = TypeAssert\classname_of(
        CLIWithRequiredArguments::class,
        static::class,
      );
      $usage .= ' '.\implode(' ', $class::getHelpTextForRequiredArguments()).
        ' ['.$class::getHelpTextForOptionalArguments().' ...]';
    } else if ($this instanceof CLIWithArguments) {
      $class = TypeAssert\classname_of(CLIWithArguments::class, static::class);
      $usage .= ' ['.$class::getHelpTextForOptionalArguments().' ...]';
    }

    $out->write($usage."\n");
    if (C\is_empty($opts)) {
      return;
    }

    $out->write("\nOptions:\n");
    foreach ($opts as $opt) {
      $short = $opt->getShort();
      $long = $opt->getLong();
      if ($opt instanceof CLIOptions\CLIOptionWithRequiredValue) {
        $long .= '=VALUE';
        if ($short !== null) {
          $short .= ' VALUE';
        }
      }

      if ($short !== null) {
        $out->write(Str\format("  -%s, --%s\n", $short, $long));
      } else {
        $out->write(Str\format("  --%s\n", $long));
      }
      $out->write(
        $opt->getHelpText()
        |> \explode("\n", $$)
        |> Vec\map(
          $$,
          $line ==> "\t".$line."\n",
        )
        |> \implode('', $$),
      );
    }
    $out->write("  -h, --help\n");
    $out->write("\tdisplay this text and exit\n");
  }
}
