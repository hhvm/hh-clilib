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

abstract class CLIOptionWithRequiredValue extends CLIOption {
  const type TSetter = (function(string):void);

  abstract protected function set(string $as_given, string $value): void;

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
