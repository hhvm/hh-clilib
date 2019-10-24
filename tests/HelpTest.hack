/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

use namespace HH\Lib\Vec;
use function Facebook\FBExpect\expect;

final class HelpTest extends TestCase {

  public function testStandalone(): void {
    $stdin = new TestLib\StringInput();
    $stdout = new TestLib\StringOutput();
    $stderr = new TestLib\StringOutput();
    $terminal = new Terminal($stdin, $stdout, $stderr);
    expect(
      () ==>
        new TestCLIWithoutArguments(vec[__FILE__, '--help'], $terminal)
    )->toThrow(ExitException::class);

    expect($stderr->getBuffer())->toBeSame('');
    $buff = $stdout->getBuffer();
    expect($buff)->toContainSubstring('--flag1');
    expect($buff)->toContainSubstring('-1');
    expect($buff)->toContainSubstring('Help text');
    expect($buff)->toContainSubstring('--flag2');
  }

  public function testAfterOptions(): void {
    $stdin = new TestLib\StringInput();
    $stdout = new TestLib\StringOutput();
    $stderr = new TestLib\StringOutput();
    $terminal = new Terminal($stdin, $stdout, $stderr);
    expect(
      () ==> new TestCLIWithoutArguments(
        vec[__FILE__, '--flag1', '-h'],
        $terminal,
      ),
    )->toThrow(ExitException::class);
    expect($stdout->getBuffer())->toContainSubstring('--flag2');
  }
}
