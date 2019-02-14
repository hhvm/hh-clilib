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
 * Adds a flag option to the CLI
 *
 * A flag has no associated required strings or enums associated with it.
 *
 * An example of a flag may be `--verbose` to increase how much output is shown
 * by your CLI.
 *
 * The `$setter` argument refers to an anonymous function that can be used
 * to set a property in the class you are using to implement your CLI.
 *
 * Here is an example of how to call this function:
 *
 * ```Hack
 * CLIOptions\flag(
 *   () ==> { $this->verbosity++; }, //
 *   "Increase output verbosity",
 *   '--verbose',
 *   '-v',
 * );
 * ```
 *
 * @param $setter A callable that can be used to set a property in the class you
 * are using to implement your CLI.
 * @param $help_text The text shown when the user provides the `--help` flag.
 * @param $long The long name for the flag. e.g., `--verbose`.
 * @param short An optional short name for the flag. e.g., `-v`.
 *
 * @return a `CLIOption` that can then be used to call the `$setter`, etc.
 *
 * @see CLIOption
 * @see CLIOptionFlag
 */
function flag(
  (function():void) $setter,
  string $help_text,
  string $long,
  ?string $short = null,
): CLIOption {
  return new CLIOptionFlag($setter, $help_text, $long, $short);
}

/**
 * Adds an option to the CLI that requires a string after it.
 *
 * An example of such an option may be `--output` that is provided a string
 * for the output directory. e.g., `--output /tmp/dir`
 *
 * The `$setter` argument refers to an anonymous function that can be used
 * to set a property in the class you are using to implement your CLI.
 *
 * Here is an example of how to call this function:
 *
 * ```Hack
 * CLIOptions\with_required_string(
 *   $s ==> { $this->outputRoot = $s; },
 *   "Directory for output files. Default: working directory",
 *   '--output',
 *   '-o',
 * ),
 * ```
 *
 * @param $setter A callable that can be used to set a property in the class you
 * are using to implement your CLI.
 * @param $help_text The text shown when the user provides the `--help` flag.
 * @param $long The long name for the flag. e.g., `--output`.
 * @param short An optional short name for the flag. e.g., `-o`.
 *
 * @return a `CLIOption` that can then be used to call the `$setter`, etc.
 *
 * @see CLIOption
 * @see CLIOptionFlag
 */
function with_required_string(
  (function(string):void) $setter,
  string $help_text,
  string $long,
  ?string $short = null,
): CLIOption {
  return new CLIOptionWithRequiredStringValue($setter, $help_text, $long, $short);
}

/**
 * Adds an option to the CLI that requires an enum of possible options after it.
 *
 * An example of such an option may be `--format` that is provided one of the
 * enum options. e.g., `--format html`
 *
 * The `$setter` argument refers to an anonymous function that can be used
 * to set a property in the class you are using to implement your CLI.
 *
 * Here is an example of how to call this function:
 *
 * ```Hack
 * enum OutputFormat: string {
 *  MARKDOWN = 'markdown';
 *  HTML = 'html';
 * }
 * CLIOptions\with_required_enum(
 *   OutputFormat::class,
 *   $f ==> { $this->format = $f; },
 *   Str\format(
 *     "Desired output format (%s). Default: %s",
 *     Str\join(OutputFormat::getValues(), '|'),
 *     (string) $this->format,
 *   ),
 *   '--format',
 *   '-f',
 * ),
 * ```
 *
 * @param $enum The enumeration with the valid values for the option.
 * @param $setter A callable that can be used to set a property in the class you
 * are using to implement your CLI.
 * @param $help_text The text shown when the user provides the `--help` flag.
 * @param $long The long name for the flag. e.g., `--output`.
 * @param short An optional short name for the flag. e.g., `-o`.
 *
 * @return a `CLIOption` that can then be used to call the `$setter`, etc.
 *
 * @see CLIOption
 * @see CLIOptionFlag
 */
function with_required_enum<T>(
  enumname<T> $enum,
  (function(T):void) $setter,
  string $help_text,
  string $long,
  ?string $short = null,
): CLIOption {
  return new CLIOptionWithRequiredEnumValue($enum, $setter, $help_text, $long, $short);
}
