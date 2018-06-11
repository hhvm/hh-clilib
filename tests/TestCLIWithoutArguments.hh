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

class TestCLIWithoutArguments extends CLIBase {
  public ?string $stringValue = null;
  public bool $flag1 = false;
  public bool $flag2 = false;
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
          $this->verbosity++;
        },
        'Increase verbosity',
        '--verbose',
        '-v',
      ),
    ];
  }

  public async function mainAsync(): Awaitable<int> {
    $this->getStdout()
      ->write(
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
