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
 * This interface provides a mechanism for providing output from the CLI.
 *
 * The Hack CLI provides default mechanisms to access stdout and stderr for
 * displaying output. And those are where this interface will usually be
 * connectd. There are other possiblities as well, and this interface allows
 * you to define those - e.g., an output mock for unit testing.
 *
 * @see CLIBase
 */
interface OutputInterface {
  /**
   * The function that is used to write the specified string to the CLI output.
   *
   * @return an integer representing the number of bytes written or a code of
   * failure.
   */
  public function write(string $_): int;
  /**
   * Returns `true` if we have hit the end of the file for the stream.
   */
  public function isEof(): bool;
}
