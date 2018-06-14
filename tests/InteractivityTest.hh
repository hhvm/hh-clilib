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

use namespace HH\Lib\{Tuple, Vec};
use function Facebook\FBExpect\expect;
use type Facebook\CLILib\TestLib\{StringInput, StringOutput};

final class InteractivityTest extends TestCase {
  private function getCLI(
  ): (TestCLIWithoutArguments, StringInput, StringOutput, StringOutput) {
    $stdin = new StringInput();
    $stdout = new StringOutput();
    $stderr = new StringOutput();
    $cli = new TestCLIWithoutArguments(
      vec[__FILE__, '--interactive'],
      new Terminal($stdin, $stdout, $stderr),
    );
    return tuple($cli, $stdin, $stdout, $stderr);
  }

  public function testClosedInput(): void {
    list($cli, $in, $out, $err) = $this->getCLI();
    $in->close();
    $ret = \HH\Asio\join($cli->mainAsync());
    expect($ret)->toBeSame(0);
    expect($out->getBuffer())->toBeSame('');
    expect($err->getBuffer())->toBeSame('');
  }

  public function testSingleCommandBeforeStart(): void {
    list($cli, $in, $out, $err) = $this->getCLI();
    $in->appendToBuffer("echo hello, world\n");
    $in->close();
    $ret = \HH\Asio\join($cli->mainAsync());
    expect($err->getBuffer())->toBeSame('');
    expect($out->getBuffer())->toBeSame("> hello, world\n");
    expect($ret)->toBeSame(0);
  }

  public function testSingleCommandAfterStart(): void {
    list($cli, $in, $out, $err) = $this->getCLI();
    list($ret, $_) = \HH\Asio\join(Tuple\from_async(
      $cli->mainAsync(),
      async {
        await \HH\Asio\later();
        expect($out->getBuffer())->toBeSame('> ');
        $out->clearBuffer();
        $in->appendToBuffer("exit 123\n");
      },
    ));
    expect($ret)->toBeSame(123);
    expect($out->getBuffer())->toBeSame('');
  }

  public function testMultipleCommandBeforeStart(): void {
    list($cli, $in, $out, $err) = $this->getCLI();
    $in->appendToBuffer("echo hello, world\n");
    $in->appendToBuffer("echo foo bar\n");
    $in->appendToBuffer("exit 123\n");
    $in->close();
    $ret = \HH\Asio\join($cli->mainAsync());
    expect($err->getBuffer())->toBeSame('');
    expect($out->getBuffer())->toBeSame("> hello, world\n> foo bar\n> ");
    expect($ret)->toBeSame(123);
  }

  public function testMultipleCommandsSequentially(): void {
    list($cli, $in, $out, $err) = $this->getCLI();
    list($ret, $_) = \HH\Asio\join(Tuple\from_async(
      $cli->mainAsync(),
      async {
        await \HH\Asio\later();
        expect($out->getBuffer())->toBeSame('> ');
        $out->clearBuffer();

        $in->appendToBuffer("echo foo bar\n");
        await \HH\Asio\later();

        expect($out->getBuffer())->toBeSame("foo bar\n> ");
        $out->clearBuffer();

        $in->appendToBuffer("echo herp derp\n");
        await \HH\Asio\later();
        expect($out->getBuffer())->toBeSame("herp derp\n> ");

        $out->clearBuffer();
        $in->appendToBuffer("exit 42\n");
      },
    ));
    expect($ret)->toBeSame(42);
    expect($out->getBuffer())->toBeSame('');
    expect($err->getBuffer())->toBeSame('');
  }
}
