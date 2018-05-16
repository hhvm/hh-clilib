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
use namespace HH\Lib\{C, Vec};

/**
 * Adds an option that requires an associated value after it.
 *
 * Where a `CLIOptionFlag` requires no value, a `CLIOptionWithRequiredValue`
 * does and an `InvalidArgumentException` will be thrown if not provided.
 *
 * @see CLIOption
 * @see CLIOptionFlag
 */
abstract class CLIOptionWithRequiredValue extends CLIOption {
  const type TSetter = (function(string):void);

  /**
   * Set the option with the provided value.
   *
   * @param $as_given The option as specified by the user. Usually matches
   *   `getShort()` or `getLong()`
   * @param $value The value specified by the user.
   */
  abstract protected function set(string $as_given, string $value): void;

  /**
   * Process user input, return new argv.
   *
   * @param $as_given - the option as specified by the user. Usually matches
   *   `getShort()` or `getLong()`
   * @param $value - value specified by the users (e.g. `--long=value`). If
   *   null, it may be appropriate to take a value from $argv
   * @param $argv - remaining unprocessed $argv
   */
  <<__Override>>
  public function apply(
    string $as_given,
    ?string $value,
    vec<string> $argv,
  ): vec<string> {
    if ($value === null) {
      if (C\is_empty($argv)) {
        throw new InvalidArgumentException(
          "option '%s' requires a value",
          $as_given,
        );
      }
      $value = C\firstx($argv);
      $argv = Vec\drop($argv, 1);
    }
    $this->set($as_given, $value);
    return $argv;
  }
}
