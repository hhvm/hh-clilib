/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

use namespace HH\Lib\{IO, Tuple, Vec};
use function Facebook\FBExpect\expect;

final class InteractivityTest extends TestCase {
  private function getCLI(
  ): (
    TestCLIWithoutArguments,
    IO\MemoryHandle,
    IO\MemoryHandle,
    IO\MemoryHandle,
  ) {
    $stdin = new IO\MemoryHandle();
    $stdout = new IO\MemoryHandle();
    $stderr = new IO\MemoryHandle();
    $cli = new TestCLIWithoutArguments(
      vec[__FILE__, '--interactive'],
      new Terminal($stdin, $stdout, $stderr),
    );
    return tuple($cli, $stdin, $stdout, $stderr);
  }

  public async function testClosedInput(): Awaitable<void> {
    list($cli, $in, $out, $err) = $this->getCLI();
    $in->close();
    $ret = await $cli->mainAsync();
    expect($ret)->toBeSame(0);
    expect($out->getBuffer())->toBeSame('> ');
    expect($err->getBuffer())->toBeSame('');
  }

  public async function testSingleCommandBeforeStart(): Awaitable<void> {
    // Don't use the MemoryHandle because reading from a closed handle
    // doesn't make sense - a pipe is both necessary, and a better
    // representation of 'echo foo | myprog'
    list($in_r, $in) = IO\pipe();
    $out = new IO\MemoryHandle();
    $err = new IO\MemoryHandle();
    $cli = new TestCLIWithoutArguments(
      vec[__FILE__, '--interactive'],
      new Terminal($in_r, $out, $err),
    );

    await $in->writeAllAsync("echo hello, world\n");
    $in->close();
    $ret = await $cli->mainAsync();
    $in_r->close();

    expect($err->getBuffer())->toEqual('');
    expect($out->getBuffer())->toEqual("> hello, world\n> ");
    expect($ret)->toEqual(0);
  }

  public async function testSingleCommandAfterStart(): Awaitable<void> {
    list($in_r, $in) = IO\pipe();
    list($out, $out_w) = IO\pipe();
    $err = new IO\MemoryHandle();
    $cli = new TestCLIWithoutArguments(
      vec[__FILE__, '--interactive'],
      new Terminal($in_r, $out_w, $err),
    );
    concurrent {
      $ret = await $cli->mainAsync();
      await async {
        expect(await $out->readAllAsync())->toEqual('> ');
        await $in->writeAllAsync("exit 123\n");
        $in->close();
      };
    }
    $in_r->close();
    $out_w->close();
    expect($ret)->toBeSame(123);
    expect(await $out->readAllAsync())->toBeSame('');
    $out->close();
  }

  public async function testMultipleCommandBeforeStart(): Awaitable<void> {
    list($cli, $in, $out, $err) = $this->getCLI();
    $in->appendToBuffer("echo hello, world\n");
    $in->appendToBuffer("echo foo bar\n");
    $in->appendToBuffer("exit 123\n");
    $ret = await $cli->mainAsync();
    expect($err->getBuffer())->toBeSame('');
    expect($out->getBuffer())->toBeSame("> hello, world\n> foo bar\n> ");
    expect($ret)->toBeSame(123);
  }

  public async function testMultipleCommandsSequentially(): Awaitable<void> {
    list($in_r, $in) = IO\pipe();
    $out = new IO\MemoryHandle();
    $err = new IO\MemoryHandle();
    $cli = new TestCLIWithoutArguments(
      vec[__FILE__, '--interactive'],
      new Terminal($in_r, $out, $err),
    );
    concurrent {
      $ret = await $cli->mainAsync();
      await async {
        await \HH\Asio\later();
        expect($out->getBuffer())->toBeSame('> ');
        $out->reset();

        await $in->writeAllAsync("echo foo bar\n");
        await \HH\Asio\later();

        expect($out->getBuffer())->toBeSame("foo bar\n> ");
        $out->reset();

        await $in->writeAllAsync("echo herp derp\n");
        await \HH\Asio\later();
        expect($out->getBuffer())->toBeSame("herp derp\n> ");

        $out->reset();
        await $in->writeAllAsync("exit 42\n");
        $in->close();
      };
    }
    expect($ret)->toBeSame(42);
    expect($out->getBuffer())->toBeSame('');
    expect($err->getBuffer())->toBeSame('');
  }
}
