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

/**
 * This class contains all of the core functionality for using the Hack CLI
 * library. It will never be instantiated directly, but instead, will invoked
 * through a call to its `main` function.
 *
 * The Hack CLI has the notion of options and arguments.
 *
 * options: An instance of the Hack CLI has a limited set of options that can be
 * used to customize the experience of its execution. For example, many CLIs
 * will have a `--help` option that can be passed to it. In the case of th Hack
 * CLI, `--help` does not even need to be explictly specified; it always exists.
 *
 * arguments: These are passed to the CLI after all the options are provided.
 * In general, the arguments are what is being executed upon.
 *
 * All non-abstract types extending this class (or any of its children), will
 * implement the definition of the abstract function `mainAsync`, which is
 * called by the `main` function. `mainAsync` will provide all the custom
 * execution when your CLI commands are invoked.
 *
 * ```Hack
 * // MyCLI.hh
 * <?hh // strict
 * final class MyCLI extends CLIBase {
 *   <<__Override>>
 *   public async function mainAsync(): Awaitable<int> {
 *     // Custom CLI handling
 *   }
 * }
 * ```
 *
 * ```Hack
 * <?hh // not strict because of top-level statements.
 * // RunMyCLI.hh
 * MyCLI::main(); // calls the main function of CLIBase
 * ```
 */
<<__ConsistentConstruct>>
abstract class CLIBase {
  private vec<string> $arguments = vec[];

  /**
   * Returns the list of options that is supported by the CLI.
   */
  abstract protected function getSupportedOptions(
  ): vec<CLIOptions\CLIOption>;

  /**
   * This is implemented by all implementations of the Hack CLI and provides
   * all of the custom execution that will occur when that CLI is invoked.
   *
   * @return  An integer representing an exit code for the process. 0 is the
   * standard for success.
   */
  abstract public function mainAsync(): Awaitable<int>;

  /**
   * Returns the list of all the arguments that were passed in any given use
   * of the CLI, including the process, flags, options and other arguments.
   *
   * In the following example:
   *
   * ```
   * hhvm bin/hh-apidoc -o /tmp ../hh-clilib
   * ```
   *
   * the `vec` returned would be:
   *
   * ```
   * vec(4) {
   *   string(13) "bin/hh-apidoc"
   *   string(2) "-o"
   *   string(4) "/tmp"
   *   string(18) "../hh-clilib"
   * }
   * ```
   */
  final public function getArgv(): vec<string> {
    return $this->argv;
  }

  /**
   * Returns the list of the arguments that were passed to the CLI after
   * all of the options have been parsed.
   *
   * In the following example:
   *
   * ```
   * hhvm bin/hh-apidoc -o /tmp ../hh-clilib
   * ```
   *
   * the `vec` returned would be:
   *
   * ```
   * vec(1) {
   *  string(18) "../hh-clilib/"
   * }
   * ```
   */
  final protected function getArguments(): vec<string> {
    invariant(
      ($_ = $this) instanceof CLIWithArguments,
      "Calling getArguments(), but don't accept arguments",
    );
    return $this->arguments;
  }

  /**
   * Determines whether your current terminal supports colors.
   *
   * This function will automatically return `true` in some non-interactive
   * cases, such as non-interactive contexts provided in continuous integration
   * (CI) systems.
   */
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

  /**
   * Determines whether the current terminal is in interactive mode.
   *
   * In general, this is `true` if the user is directly typing into stdin.
   */
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

  /**
   * Gets the standard process output for the current CLI.
   *
   * By default, this is the process standard output, or file descriptor 1.
   */
  final protected function getStdout(): OutputInterface {
    return $this->stdout;
  }

  /**
   * Gets the standard error output for the current CLI.
   *
   * By default, this is the process standard error, or file descriptor 2.
   *
   * This is usually a wrapper around stdout, and should be used instead of
   * direct access.
   */
  final protected function getStderr(): OutputInterface {
    return $this->stderr;
  }

  /**
   * This is usually the main entry point to create an instance of a Hack CLI.
   * By default, the output is to stdout and errors are written to stderr. But
   * those can be overriden by the actual concrete instance of this abstract
   * class.
   *
   * This function does not return anything. Instead `exit()` is called with an
   * exit code for success and failure.
   *
   * @see CLIBase::mainAsync
   */
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

  /**
   * This creates a new instance of the Hack CLI.
   *
   * This will generally be called indirectly via a call to `main()`. However,
   * a direct call to this constructor may be useful in certain cases, such as
   * mocks and unit tests.
   */
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
      if ($this instanceof CLIWithRequiredArguments) {
        $class = TypeAssert\classname_of(
          CLIWithRequiredArguments::class,
          static::class,
        );
        throw new InvalidArgumentException(
          "%s must be specified.",
          Str\join($class::getHelpTextForRequiredArguments(), ' '),
        );
      }
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

  /**
   * Displays the help and usage information for this CLI.
   *
   * The help information is automatically generated. The default generation
   * can be overridden - but generally not necessary.
   */
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
