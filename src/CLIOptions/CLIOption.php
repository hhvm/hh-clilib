<?hh
/**
 * Copyright (c) 2017, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the "hack" directory of this source tree. An additional
 * grant of patent rights can be found in the PATENTS file in the same
 * directory.
 */

namespace Facebook\HHAST\__Private\CLIOptions;

use namespace HH\Lib\Str;

abstract class CLIOption {
  private string $long;
  private ?string $short = null;

  public function __construct(
    private string $helpText,
    string $long,
    ?string $short,
  ) {
    invariant(
      Str\starts_with($long, '--'),
      "long argument '%s' doesn't start with '--'",
      $long,
    );
    $this->long = Str\strip_prefix($long, '--');
    if ($short === null) {
      return;
    }

    invariant(
      Str\starts_with($short, '-'),
      "short argument '%s' doesn't start with '-'",
      $short,
    );
    invariant(
      !Str\starts_with($short, '--'),
      "short argument '%s' shouldn't start with '--'",
      $short,
    );
    $this->short = Str\strip_prefix($short, '-');
  }

  final public function getHelpText(): string {
    return $this->helpText;
  }

  final public function getLong(): string {
    return $this->long;
  }

  final public function getShort(): ?string {
    return $this->short;
  }

  /**
   * Process user input, return new argv.
   *
   * @param $as_given - the option as specified by the user. Usually matches
   *   `getShort()` or `getLong()`
   * @param $value - value specified by the users (e.g. `--long=value`). If
   *   null, it may be appropriate to take a value from $argv
   * @param $argv - remaining unprocessed $argv
   */
  abstract public function apply(
    string $as_given,
    ?string $value,
    vec<string> $argv,
  ): vec<string>;
}
