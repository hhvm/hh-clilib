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

namespace Facebook\CLILib;

use namespace HH\Lib\Str;

abstract class CLIException extends \Exception {
  public function __construct(
    Str\SprintfFormatString $message,
    mixed ...$args
  ) {
    parent::__construct(/* HH_IGNORE_ERROR[4027] */ Str\format($message, ...$args));
  }
}

