<?hh
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

  protected function set(string $_as_given, string $value): void {
    $setter = $this->setter;
    $setter($value);
  }
}
