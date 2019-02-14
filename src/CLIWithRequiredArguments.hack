/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;
use namespace HH\Lib\C;

/**
 * A class to represent a CLI that has required arguments associated with it.
 *
 * Arguments are strings passed to the CLI after all flags and options are
 * provided.
 *
 * e.g., here the argument would be `../hh-clilib`
 *
 * ```
 * hhvm bin/hh-apidoc -o /tmp ../hh-clilib
 * ```
 *
 * This class adds one additional function to the parent `CLIWithArguments`.
 *
 * @see CLIWithArguments
 * @see CLIBase
 */
abstract class CLIWithRequiredArguments extends CLIWithArguments {
  /**
   * For arguments that are required for the CLI, this provides a `vec` of
   * the help text associated with those arguments.
   */
  abstract public static function getHelpTextForRequiredArguments(
  ): vec<string>;

  /**
   * For arguments that are optional to the CLI, this function will provide
   * their help text.
   *
   * The default return is the first string in the required argument `vec` -
   * this can be overridden for more detailed information.
   *
   */
  <<__Override>>
  public static function getHelpTextForOptionalArguments(): string {
    return C\firstx(static::getHelpTextForRequiredArguments());
  }
}
