/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\CLIOptions;


/**
 * Creates a CLI option that requrires an associated string value.
 *
 * e.g., `--outputDir /tmp/dir`, where `/tmp/dir` would be the associated
 * string.
 *
 * @see CLIOption
 */
final class CLIOptionWithRequiredStringValue extends CLIOptionWithRequiredValue {
  const type TSetter = (function(string):void);

  /**
   * Creates the option with the required string.
   *
   * @param $setter A callable that can be used to set a property in the class
   * you are using to implement your CLI.
   * @param $help_text The text shown when the user provides the `--help` flag.
   * @param $long The long name for the flag. e.g., `--output`.
   * @param short An optional short name for the flag. e.g., `-o`.
   */
  public function __construct(
    private self::TSetter $setter,
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
   * @param $as_given The option as specified by the user. Usually matches
   *   `getShort()` or `getLong()`
   * @param $value The value specified by the user.
   */
  <<__Override>>
  protected function set(string $_as_given, string $value): void {
    $setter = $this->setter;
    $setter($value);
  }
}
