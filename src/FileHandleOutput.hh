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

use namespace HH\Lib\Str;

/**
 * This class is provided by the Hack CLI as implementation of `OutputInterface`
 * to allow writing to file handles, including stdout and stderr.
 *
 * @see FileHandleInput
 * @see OutputInterface
 * @see CLIBase
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
    \stream_set_blocking($f, false);
  }

  /**
   * Write the output to the specified file handle.
   *
   * @return The number of bytes written or a code of failure.
   */
  public function rawWrite(string $text): int {
    return \fwrite($this->f, $text);
  }

  public function write(string $text): int {
    return $this->rawWrite($text);
  }

  public async function writeAsync(string $text): Awaitable<void> {
    while ($text !== '') {
      $written = $this->rawWrite($text);
      $text = Str\slice($text, $written);
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop] */
      await \stream_await($this->f, \STREAM_AWAIT_WRITE);
    }
  }

  /**
   * Returns `true` if we have hit the end of the file for the file handle.
   */
  public function isEof(): bool {
    return \feof($this->f);
  }
}
