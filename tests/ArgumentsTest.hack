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

final class ArgumentsTest extends TestCase {
  public function testProvidingArgumentsToCLIWithoutArguments(): void {
    expect(() ==> {
      self::makeCLI(TestCLIWithoutArguments::class, 'foo');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testNoArgumentToCLIWithOptionalArguments(): void {
    list($cli, $_, $_) = self::makeCLI(TestCLIWithOptionalArguments::class);
    expect($cli->getArgumentsForTesting())->toBeEmpty();
  }

  public function testArgumentToCLIWithOptionalArguments(): void {
    list($cli, $_, $_) =
      self::makeCLI(TestCLIWithOptionalArguments::class, 'foo');
    expect($cli->getArgumentsForTesting())->toBeSame(vec['foo']);
  }

  public function testArgumentAfterDashDash(): void {
    list($cli, $_, $_) =
      self::makeCLI(TestCLIWithOptionalArguments::class, '--', 'foo');
    expect($cli->getArgumentsForTesting())->toBeSame(vec['foo']);
  }

  public function testMultipleDashDash(): void {
    list($cli, $_, $_) = self::makeCLI(
      TestCLIWithOptionalArguments::class,
      'foo',
      '--',
      'bar',
      '--',
      'baz',
    );
    expect($cli->getArgumentsForTesting())->toBeSame(
      vec['foo', 'bar', '--', 'baz'],
    );
  }

  public function testArgumentsAfterOptions(): void {
    list($cli, $_, $_) = self::makeCLI(
      TestCLIWithOptionalArguments::class,
      '--string',
      'foo',
      'bar',
    );
    expect($cli->stringValue)->toBeSame('foo');
    expect($cli->getArgumentsForTesting())->toBeSame(vec['bar']);
  }

  public function testArgumentCollidingWithOption(): void {
    list($cli, $_, $_) = self::makeCLI(
      TestCLIWithOptionalArguments::class,
      '--',
      '--string',
      'foo',
    );
    expect($cli->stringValue)->toBeNull();
    expect($cli->getArgumentsForTesting())->toBeSame(vec['--string', 'foo']);
  }

  public function testHelpAfterDashDash(): void {
    list($cli, $stdout, $stderr) =
      self::makeCLI(TestCLIWithOptionalArguments::class, '--', '--help');
    expect($cli->getArgumentsForTesting())->toBeSame(vec['--help']);
    expect($stdout->getBuffer())->toBeSame('');
    expect($stderr->getBuffer())->toBeSame('');
  }

  public function testNoArgumentsForCLIWithRequiredArguments(): void {
    expect(() ==> {
      self::makeCLI(TestCLIWithRequiredArguments::class);
    })->toThrow(InvalidArgumentException::class);
  }

  public function testArgumentsForCLIWithRequiredArguments(): void {
    list($cli, $_, $_) = self::makeCLI(
      TestCLIWithRequiredArguments::class,
      '--string',
      'foo',
      '--',
      'bar',
      '--flag1',
      'baz',
    );
    expect($cli->getArgumentsForTesting())->toBeSame(
      vec['bar', '--flag1', 'baz'],
    );
    expect($cli->flag1)->toBeFalse();
    expect($cli->stringValue)->toBeSame('foo');
  }
}
