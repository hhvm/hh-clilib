<?hh
/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\CLIOptions;

use type Facebook\CLILib\InvalidArgumentException;

final class CLIOptionFlag extends CLIOption {
  const type TSetter = (function():void);

  public function __construct(
    private self::TSetter $setter,
    string $help_text,
    string $long,
    ?string $short,
  ) {
    parent::__construct($help_text, $long, $short);
  }

  public function set(): void {
    $setter = $this->setter;
    $setter();
  }

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
