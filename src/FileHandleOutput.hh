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

final class FileHandleOutput implements OutputInterface {
  public function __construct(
    private resource $f,
  ) {
  }

  public function write(string $text): int {
    return \fwrite($this->f, $text);
  }

  public function isEof(): bool {
    return \feof($this->f);
  }
}
