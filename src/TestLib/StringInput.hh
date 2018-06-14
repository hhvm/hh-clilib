<?hh // strict
/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\TestLib;

use type Facebook\CLILib\InputInterface;
use namespace HH\Lib\{Math, Str};

/** This class stores all CLI output in a string.
 *
 * It is intended for unit testing.
 *
 * @see FileHandleInput
 */
final class StringInput implements InputInterface {
  private string $buffer = '';
  private bool $isClosed = false;

  private async function waitForDataAsync(): Awaitable<void> {
    while (true) {
      if ($this->buffer !== '' || $this->isClosed) {
        return;
      }
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop] */
      await \HH\Asio\usleep(100);
    }
  }

  public function appendToBuffer(string $data): void {
    $this->buffer .= $data;
  }

  public function getBuffer(): string {
    return $this->buffer;
  }

  public async function readAsync(?int $max_bytes = null): Awaitable<string> {
    invariant(
      $max_bytes === null || $max_bytes >= 0,
      '$max_bytes must be null or non-negative',
    );
    await $this->waitForDataAsync();

    $buf = $this->buffer;
    if ($max_bytes === null || $max_bytes > Str\length($this->buffer)) {
      $this->buffer = '';
      return $buf;
    }

    $this->buffer = Str\slice($buf, $max_bytes);
    return Str\slice($buf, 0, $max_bytes);
  }

  public async function readLineAsync(?int $max_bytes = null): Awaitable<string> {
    invariant(
      $max_bytes === null || $max_bytes >= 0,
      '$max_bytes must be null or non-negative',
    );
    await $this->waitForDataAsync();

    $buf = $this->buffer;
    $pos = Str\search($buf, "\n");
    if ($pos === null) {
      $this->buffer = '';
      return $buf;
    }

    if ($max_bytes !== null) {
      $pos = Math\minva($pos, $max_bytes);
    }
    $this->buffer = Str\slice($buf, $pos + 1);
    return Str\slice($buf, 0, $pos);
  }

  public function close(): void {
    $this->isClosed = true;
  }

  public function isEof(): bool {
    return $this->buffer === '' && $this->isClosed;
  }
}
