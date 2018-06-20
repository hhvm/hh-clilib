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
 * This class is provided by the Hack CLI as implementation of `InputInterface`
 * to allow writing to file handles, including stdin.
 *
 * @see InputInterface
 * @see FileHandleOutput
 * @see CLIBase
 */
final class FileHandleInput implements InputInterface {
  /**
   * Construct a `FileHandleInput` by passing a file resource.
   *
   * This will usually be the `\STDIN` resource.
   *
   * This API is currently unstable. It will change when our currently
   * experimental `FileSystem` API becomes part of the Hack Standard Library.
   */
  public function __construct(private resource $f) {
    \stream_set_blocking($f, false);
  }

  private async function waitForDataAsync(): Awaitable<void> {
    // `stream_await` without a timeout is broken:
    //   https://github.com/facebook/hhvm/issues/8230
    // So, why this number? `stream_await` takes a double of seconds, but
    // internally, FileAwait converts it into an int64_t of milliseconds. This
    // means that the largest timeout is PHP_INT_MAX ms, so /1000 to get
    // seconds, then fudge a little because of floating point precision
    $magic = (\PHP_INT_MAX / 1000) - 1.1;
    await \stream_await($this->f, \STREAM_AWAIT_READ, $magic);
  }

  /**
   * Read until we reach `$max_bytes` or the end of the file.
   *
   * @param $max_bytes must be null or >= 0
   */
  public async function readAsync(?int $max_bytes = null): Awaitable<string> {
    invariant(
      $max_bytes === null || $max_bytes >= 0,
      '$max_bytes must be null or non-negative',
    );
    if ($max_bytes === 0) {
      return '';
    }
    while (true) {
      $data = \stream_get_contents($this->f, $max_bytes ?? -1);
      if ($data !== '' || $this->isEof()) {
        return $data;
      }
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop] */
      await $this->waitForDataAsync();
    }
  }

  /**
   * Read until we reach a newline, the end of the file, or `$max_bytes` bytes.
   *
   * @param $max_bytes must be null or >= 0.
   */
  public async function readLineAsync(
    ?int $max_bytes = null,
  ): Awaitable<string> {
    invariant(
      $max_bytes === null || $max_bytes >= 0,
      '$max_bytes must be null or non-negative',
    );

    if ($max_bytes === 0) {
      return '';
    }

    if ($max_bytes === null) {
      // PHP does not document the default value
      $impl = () ==> \fgets($this->f);
    } else {
      // However, it does document that up to `$length - 1` bytes are returned
      $impl = () ==> \fgets($this->f, $max_bytes + 1);
    }
    $data = $impl();
    while ($data === false && !$this->isEof()) {
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop] */
      await $this->waitForDataAsync();
      $data = $impl();
    }
    return $data === false ? '' : $data;
  }

  /**
   * Returns `true` if we have hit the end of the file for the file handle.
   */
  public function isEof(): bool {
    return \feof($this->f);
  }
}
