/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

use namespace HH\Lib\{IO, Vec};
use function Facebook\FBExpect\expect;

abstract class TestCase extends \Facebook\HackTest\HackTest {
  protected static function makeCLI<T as CLIBase>(
    classname<T> $cli, 
    string ...$argv
  ): (T, IO\MemoryHandle, IO\MemoryHandle) {
    // $argv[0] is the executable
    $args = Vec\concat(vec[__FILE__], $argv);
    $stdin = new IO\MemoryHandle();
    $stdout = new IO\MemoryHandle();
    $stderr = new IO\MemoryHandle();
    $terminal = new Terminal($stdin, $stdout, $stderr);
    return tuple(
      new $cli($args, $terminal),
      $stdout,
      $stderr,
    );
  }
}
