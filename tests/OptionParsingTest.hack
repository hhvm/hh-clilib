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

final class OptionParsingTest extends TestCase {
  private static function cli(
    string ...$argv
  ): (TestCLIWithoutArguments, IO\MemoryHandle, IO\MemoryHandle) {
    return self::makeCLI(TestCLIWithoutArguments::class, ...$argv);
  }

  public function testNoOptions(): void {
    list($cli, $_, $_) = self::cli();
    expect($cli->stringValue)->toBeNull();
    expect($cli->flag1)->toBeFalse();
    expect($cli->flag2)->toBeFalse();
    expect($cli->verbosity)->toBeSame(0);
  }

  public function testLongFlag(): void {
    list($cli, $stdout, $_) = self::cli('--flag1');
    expect($cli->flag1)->toBeTrue();
  }

  public function testMultipleLongFlags(): void {
    list($cli, $_, $_) = self::cli('--flag1', '--flag2');
    expect($cli->flag1)->toBeTrue();
    expect($cli->flag2)->toBeTrue();
  }

  public function testRepeatLongFlag(): void {
    list($cli, $_, $_) = self::cli('--verbose', '--verbose');
    expect($cli->verbosity)->toBeSame(2);
  }

  public function testShortFlag(): void {
    list($cli, $_, $_) = self::cli('-1');
    expect($cli->flag1)->toBeTrue();
  }

  public function testMultipleShortFlags(): void {
    list($cli, $_, $_) = self::cli('-1', '-2');
    expect($cli->flag1)->toBeTrue();
    expect($cli->flag2)->toBeTrue();
  }

  public function testRepeatShortFlag(): void {
    list($cli, $_, $_) = self::cli('-v', '-v');
    expect($cli->verbosity)->toBeSame(2);
  }

  public function testWithMissingValue(): void {
    expect(() ==> {
      self::cli('--string');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testLongOptionWithValueAsNextArgument(): void {
    list($cli, $_, $_) = self::cli('--string', 'foo', '-1');
    expect($cli->stringValue)->toBeSame('foo');
    expect($cli->flag1)->toBeTrue();
  }

  public function testLongOptionWithValueAfterEquals(): void {
    list($cli, $_, $_) = self::cli('--string=foo', '-1');
    expect($cli->stringValue)->toBeSame('foo');
    expect($cli->flag1)->toBeTrue();
  }

  public function testShortOptionWithValueAsNextArgument(): void {
    list($cli, $_, $_) = self::cli('-s', 'foo', '-1');
    expect($cli->stringValue)->toBeSame('foo');
    expect($cli->flag1)->toBeTrue();
  }

  public function testShortOptionWithValueAfterEquals(): void {
    list($cli, $_, $_) = self::cli('-s=foo', '-1');
    expect($cli->stringValue)->toBeSame('foo');
    expect($cli->flag1)->toBeTrue();
  }

  public function testOptionWithUnwantedValueAfterEquals(): void {
    expect(() ==> {
      self::cli('--flag1=foo');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testOptionWithUnwantedValueAsNextArgument(): void {
    expect(() ==> {
      self::cli('--flag1', 'foo');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testInvalidLongOption(): void {
    expect(() ==> {
      self::cli('--invalid');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testInvalidShortOption(): void {
    expect(() ==> {
      self::cli('-i');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testDashDashAsRequiredValueAsNextArgument(): void {
    expect(() ==> {
      self::cli('--string', '--');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testDashDashAsRequiredValueAfterEquals(): void {
    list($cli, $_, $_) = self::cli('--string=--');
    expect($cli->stringValue)->toBeSame('--');
  }

  public function testMultipleShortOptionsWithAssignment(): void {
    expect(() ==> {
      self::cli('-vs=something');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testMultipleShortOptionsWithRequiredValue(): void {
    expect(() ==> {
      self::cli('-sv');
    })->toThrow(InvalidArgumentException::class);
  }

  public function testMultipleShortOptions(): void {
    list($cli, $_, $_) = self::cli('-12v');
    expect($cli->flag1)->toBeTrue();
    expect($cli->flag2)->toBeTrue();
    expect($cli->verbosity)->toBeSame(1);
  }

  public function testMultipleEqualShortOptionsBehaveAsSingle(): void {
    list($cli, $_, $_) = self::cli('-vvv');
    expect($cli->verbosity)->toBeSame(1);
  }
}
