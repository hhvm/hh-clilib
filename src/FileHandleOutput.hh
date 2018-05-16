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
 * This class is provided by the Hack CLI as implementation of `OutputInterface`
 * to allow writing to file handles, including stdout and stderr.
 *
 * See @OutputInterface
 * See @CLIBase
 */
final class FileHandleOutput implements OutputInterface {
  /**
   * Construct a FileHandleOutput by passing a file resource.
   *
   * `\STDOUT` and `\STDERR` are resources for stdin and stderr
   *
   * This API is currently unstable. It will change when our currently
   * experimental `FileSystem` API becomes part of the Hack Standard Library.
   */
  public function __construct(
    private resource $f,
  ) {
  }

  /**
   * Write the output to the specified file handle.
   *
   * @return The number of bytes written or a code of failure.
   */
  public function write(string $text): int {
    return \fwrite($this->f, $text);
  }

  /**
   * Returns `true` if we have hit the end of the file for the file handle.
   */
  public function isEof(): bool {
    return \feof($this->f);
  }
}
