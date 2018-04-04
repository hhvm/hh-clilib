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
use namespace HH\Lib\{C, Str, Vec};

final class CLIOptionWithRequiredEnumValue<T> extends CLIOptionWithRequiredValue {
  public function __construct(
    private enumname<T> $enumname,
    private (function(T): void) $setter,
    string $help_text,
    string $long,
    ?string $short,
  ) {
    parent::__construct($help_text, $long, $short);
  }

  protected function set(string $as_given, string $value): void {
    $enum = $this->enumname;
    if (!$enum::isValid($value)) {
      throw new InvalidArgumentException(
        "'%s' is not a valid value for '%s' - valid values are: %s",
        $value,
        $as_given,
        $enum::getValues()
          |> Vec\map($$, $v ==> (string) $v)
          |> Str\join($$, ' | '),
      );
    }
    $setter = $this->setter;
    $setter($enum::assert($value));
  }
}
