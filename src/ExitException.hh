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

/**
 * An `Exception` used when you want to explicitly exit the running of the CLI
 * process and provide a specific exit code.
 *
 * In real usage, the exception handler is likely to call `exit()`` with the
 * specified exit code.
 *
 * @see CLIException
 */
final class ExitException extends CLIException {
  /**
   * Create the exception with the exit code.
   *
   * Note that success via `new ExitException(0)` is valid usage. For example,
   * you may throw this exception if you are jumping out of the CLI process
   * because you specified a certain flag.
   */
  public function __construct(
    int $code,
  ) {
    $this->code = $code;
    parent::__construct('exit(%d)', $code);
  }
}
