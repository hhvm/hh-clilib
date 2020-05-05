/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\TestLib;


use namespace HH\Lib\{IO, Str};

/** This class stores all CLI output in a string */
final class StringOutput implements IO\WriteHandle, IO\UserspaceHandle {
  private string $buffer = '';

  public function rawWriteBlocking(string $data): int {
    return $this->write($data);
  }

  public function write(string $data): int {
    $this->buffer .= $data;
    return Str\length($data);
  }

  public async function writeAsync(
    string $data,
    ?num $_timeout_mixed = null,
  ): Awaitable<void> {
    $this->write($data);
  }

  public function isEndOfFile(): bool {
    return false;
  }

  public async function flushAsync(): Awaitable<void> {
    // nothing to do here
  }

  public async function closeAsync(): Awaitable<void> {
    await $this->flushAsync();
  }

  public function getBuffer(): string {
    return $this->buffer;
  }

  public function clearBuffer(): void {
    $this->buffer = '';
  }
}
