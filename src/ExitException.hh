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

/**
 * An `Exception` used when running of the CLI resulted in something other than
 * success.
 *
 * @see CLIException
 */
final class ExitException extends CLIException {
  /**
   * This exception is created via an exit code, that is usually non-zero, as
   * zero genreally represents success.
   */
  public function __construct(
    int $code,
  ) {
    $this->code = $code;
    parent::__construct('exit(%d)', $code);
  }
}
