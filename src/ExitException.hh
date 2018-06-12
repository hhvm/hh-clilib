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

final class ExitException extends CLIException {
  public function __construct(
    int $code,
  ) {
    $this->code = $code;
    parent::__construct('exit(%d)', $code);
  }
}
