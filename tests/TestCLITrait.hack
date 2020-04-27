/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

use namespace HH\Lib\Str;

trait TestCLITrait {
  require extends CLIBase;

  public ?string $stringValue = null;
  public bool $flag1 = false;
  public bool $flag2 = false;
  private bool $interactive = false;
  public int $verbosity = 0;

  public function getSupportedOptions(): vec<CLIOptions\CLIOption> {
    return vec[
      CLIOptions\with_required_string(
        $s ==> {
          $this->stringValue = $s;
        },
        'Help text for string',
        '--string',
        '-s',
      ),
      CLIOptions\flag(
        () ==> {
          $this->flag1 = true;
        },
        'Help text for flag1',
        '--flag1',
        '-1',
      ),
      CLIOptions\flag(
        () ==> {
          $this->flag2 = true;
        },
        'Help text for flag2',
        '--flag2',
        '-2',
      ),
      CLIOptions\flag(
        () ==> {
          $this->interactive = true;
        },
        'Enable interactive mode',
        '--interactive',
      ),
      CLIOptions\flag(
        () ==> {
          $this->verbosity++;
        },
        'Increase verbosity',
        '--verbose',
        '-v',
      ),
    ];
  }

  private async function replAsync(): Awaitable<int> {
    // TODO: Replace with a buffered line reader when new hsl-experimental is
    // released.
    $in = $this->getStdin() as TestLib\StringInput;
    $out = $this->getStdout();
    $err = $this->getStderr();
    while (!$in->isEndOfFile()) {
      await $out->writeAsync('> ');
      /* HHAST_IGNORE_ERROR[DontAwaitInALoop */
      $line = await $in->readLineAsync();
      $sep = Str\search($line, ' ');
      if ($sep === null) {
        await $err->writeAsync("Usage: (exit <code>|echo foo bar ....\n");
        return 1;
      }
      $command = Str\slice($line, 0, $sep);
      $args = Str\slice($line, $sep + 1) |> Str\strip_suffix($$, "\n");
      switch ($command) {
        case 'echo':
          await $out->writeAsync($args."\n");
          break;
        case 'exit':
          $code = Str\to_int($args);
          if ($code === null) {
            await $err->writeAsync("Exit code must be numeric.\n");
            return 1;
          }
          return $code;
        default:
          await $err->writeAsync("Invalid command\n");
          return 1;
      }
    }
    return 0;
  }

  public async function mainAsync(): Awaitable<int> {
    if ($this->interactive) {
      return await $this->replAsync();
    }
    await $this->getStdout()
      ->writeAsync(
        \json_encode(
          dict[
            'string' => $this->stringValue,
            'flag1' => $this->flag1,
            'flag2' => $this->flag2,
            'verbosity' => $this->verbosity,
          ],
          \JSON_PRETTY_PRINT,
        ),
      );
    return 0;
  }
}
