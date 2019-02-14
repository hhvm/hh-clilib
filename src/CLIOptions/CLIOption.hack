/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\CLIOptions;

use namespace HH\Lib\Str;

enum CLIOptionType: int {
  LONG = 1;
  SHORT = 2;
  ARGUMENT = 3;
}

/**
 * This class represents an individual option that can be provided to the Hack
 * CLI.
 *
 * Long versions of an option start with `--`; short versions start with `-`.
 */
abstract class CLIOption {
  private string $long;
  private ?string $short = null;

  /**
   * Create an option for the CLI.
   *
   * An option is created by a name and text to show when help is requested.
   * Optionally, a short name can be provided as well.
   *
   * @param $helpText The string to show for this option when `--help` is used.
   * @param $long The long name for the option. It is the name used with `--`.
   * @param $short The optional short name for the option. It is the name used
   *   with `-`.
   */
  public function __construct(
    private string $helpText,
    string $long,
    ?string $short,
  ) {
    invariant(
      Str\starts_with($long, '--'),
      "long argument '%s' doesn't start with '--'",
      $long,
    );
    $this->long = Str\strip_prefix($long, '--');
    invariant(
      Str\length($this->long) > 0,
      'long argument length should be greater than zero',
    );
    if ($short === null) {
      return;
    }

    invariant(
      Str\starts_with($short, '-'),
      "short argument '%s' doesn't start with '-'",
      $short,
    );
    invariant(
      !Str\starts_with($short, '--'),
      "short argument '%s' shouldn't start with '--'",
      $short,
    );
    $this->short = Str\strip_prefix($short, '-');
    invariant(
      Str\length((string)$this->short) === 1,
      "short argument '%s' length should be equal to 1",
      $short,
    );
  }

  /** @selfdocumenting */
  final public function getHelpText(): string {
    return $this->helpText;
  }

  /**
   * Get the long name associated with this option.
   */
  final public function getLong(): string {
    return $this->long;
  }

  /**
   * Get the short name associated with this option.
   *
   * If there is no short name, then `null` is returned.
   */
  final public function getShort(): ?string {
    return $this->short;
  }

  /**
  * Get type and value based on the option provided.
  *
  * Long options start with ``--`, whereas short options start with `-`
  */
  final public static function getTypeAndValue(
    string $option,
  ): (CLIOptionType, string) {
    if (Str\starts_with($option, '--')) {
      return tuple(CLIOptionType::LONG, Str\strip_prefix($option, '--'));
    } else if (Str\starts_with($option, '-')) {
      return tuple(CLIOptionType::SHORT, Str\strip_prefix($option, '-'));
    }
    return tuple(CLIOptionType::ARGUMENT, $option);
  }

  /**
   * Process user input, return new argv.
   *
   * @param $as_given - the option as specified by the user. Usually matches
   *   `getShort()` or `getLong()`
   * @param $value - value specified by the users (e.g. `--long=value`). If
   *   null, it may be appropriate to take a value from $argv
   * @param $argv - remaining unprocessed $argv
   *
   * @return the new $argv after this option is applied.
   */
  abstract public function apply(
    string $as_given,
    ?string $value,
    vec<string> $argv,
  ): vec<string>;
}
