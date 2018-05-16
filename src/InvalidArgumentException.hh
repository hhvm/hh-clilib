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
 * This class is used to pass an exception when an argument passed to the CLI
 * is not a valid, expected argument.
 *
 * @see CLIException
 */
final class InvalidArgumentException extends CLIException {
}
