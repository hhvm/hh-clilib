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
