<?hh // strict
/**
 * Copyright (c) 2017, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the "hack" directory of this source tree. An additional
 * grant of patent rights can be found in the PATENTS file in the same
 * directory.
 */

namespace Facebook\CLILib\CLIOptions;

use type Facebook\CLILib\InvalidArgumentException;

/**
 * Adds a flag option to the CLI.
 *
 * A flag has no associated required values associated with it.
 *
 * A wrapper for this class is provided in the
 * `Facebook\CLILib\CLIOptions\flag()` function.
 *
 */
final class CLIOptionFlag extends CLIOption {
  const type TSetter = (function():void);

  /**
   * Create the flag.
   *
   * @param $setter A callable that can be used to set a property in the class
   * you are using to implement your CLI.
   * @param $help_text The text shown when the user provides the `--help` flag.
   * @param $long The long name for the flag.
   * @param short An optional short name for the flag.
   *
   * @return a `CLIOption` that can then be used to call the `$setter`, etc.
   *
   * @see CLIOption
   */
  public function __construct(
    private self::TSetter $setter,
    string $help_text,
    string $long,
    ?string $short,
  ) {
    parent::__construct($help_text, $long, $short);
  }

  /**
   * The setter for the flag.
   *
   * The setter is provided as a callable when the flag is constructed.
   */
  public function set(): void {
    $setter = $this->setter;
    $setter();
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
  <<__Override>>
  public function apply(
    string $as_given,
    ?string $value,
    vec<string> $argv,
  ): vec<string> {
    if ($value !== null) {
      throw new InvalidArgumentException(
        "'%s' specifies a value, however values aren't supported for that ".
        "option",
        $as_given,
      );
    }
    $this->set();
    return $argv;
  }
}
