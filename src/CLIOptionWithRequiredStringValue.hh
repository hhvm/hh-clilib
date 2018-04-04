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

use namespace HH\Lib\{C, Vec};

final class CLIOptionWithRequiredStringValue extends CLIOptionWithRequiredValue {
  const type TSetter = (function(string):void);

  public function __construct(
    private self::TSetter $setter,
    string $help_text,
    string $long,
    ?string $short,
  ) {
    parent::__construct($help_text, $long, $short);
  }

  protected function set(string $value): void {
    $setter = $this->setter;
    $setter($value);
  }
}
