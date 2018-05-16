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

/**
 * This is the class that should be used whenever an exception is thrown
 * related to the functionality of the Hack CLI.
 *
 * @see Exception
 */
abstract class CLIException extends \Exception {
  /**
   * Use this to construct a `CLIException`
   *
   * @param $message - The message to show a user when an exception is thrown.
   * @param $args - Arguments that are used within the message string.
   *
   * @see Str\format
   */
  public function __construct(
    Str\SprintfFormatString $message,
    mixed ...$args
  ) {
    parent::__construct(/* HH_IGNORE_ERROR[4027] */ Str\format($message, ...$args));
  }
}
