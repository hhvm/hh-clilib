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

use type Facebook\CLILib\OutputInterface;

use namespace HH\Lib\Str;

/** This class stores all CLI output in a string */
final class StringOutput implements OutputInterface {
  private string $buffer = '';

  public function rawWrite(string $data): int {
    $this->buffer .= $data;
    return Str\length($data);
  }

  public function write(string $data): int {
    return $this->rawWrite($data);
  }

  public async function writeAsync(string $data): Awaitable<void> {
    $this->buffer .= $data;
  }

  public function isEof(): bool {
    return false;
  }

  public function getBuffer(): string {
    return $this->buffer;
  }

  public function clearBuffer(): void{
    $this->buffer = '';
  }
}
