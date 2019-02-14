/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\CLIOptions;

use type Facebook\CLILib\InvalidArgumentException;
use namespace HH\Lib\{Str, Vec};

/**
 * Creates a CLI option that requrires a value from a provided enum type.
 *
 * e.g., `--outputType html`, where `html` would be the associated enum value,
 * in an enum that may be defined as:
 *
 * ```Hack
 * enum OutputFormat: string {
 *   MARKDOWN = 'markdown';
 *   HTML = 'html';
 * }
 * ```
 *
 * A wrapper for this class is provided in `CLIOptions`.
 *
 * @see CLIOptions
 * @see CLIOption
 */
final class CLIOptionWithRequiredEnumValue<T> extends CLIOptionWithRequiredValue {
  /**
   * Creates the option with the required enum that has the valid values.
   *
   * The enum should be created with the `::class` magic constant, in the
   * form of `MyEnum::class`.
   *
   * @param $enumname The enum containing the valid values.
   * @param $setter A callable that can be used to set a property in the class
   * you are using to implement your CLI.
   * @param $help_text The text shown when the user provides the `--help` flag.
   * @param $long The long name for the flag. e.g., `--output`.
   * @param short An optional short name for the flag. e.g., `-o`.
   */
  public function __construct(
    private enumname<T> $enumname,
    private (function(T): void) $setter,
    string $help_text,
    string $long,
    ?string $short,
  ) {
    parent::__construct($help_text, $long, $short);
  }

  /**
   * Set the option with the provided value, using the setter provided when
   * the option was created.
   *
   * The value must be a valid value in the enum.
   *
   * @param $as_given The option as specified by the user. Usually matches
   *   `getShort()` or `getLong()`
   * @param $value The value specified by the user.
   */
  <<__Override>>
  protected function set(string $as_given, string $value): void {
    $enum = $this->enumname;
    if (!$enum::isValid($value)) {
      throw new InvalidArgumentException(
        "'%s' is not a valid value for '%s' - valid values are: %s",
        $value,
        $as_given,
        $enum::getValues()
          |> Vec\map($$, $v ==> (string) $v)
          |> Str\join($$, ' | '),
      );
    }
    $setter = $this->setter;
    $setter($enum::assert($value));
  }
}
