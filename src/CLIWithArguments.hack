/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

/**
 * A class to represent a CLI that has arguments associated with it.
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
 * This class adds one additional function to the parent `CLIBase`.
 *
 * @see CLIBase
 */
abstract class CLIWithArguments extends CLIBase {
  /**
   * For arguments that are optional to the CLI, this function will provide
   * their help text.
   *
   * The default return is `ARGUMENT` - this can be overridden for more detailed
   * information.
   *
   */
  public static function getHelpTextForOptionalArguments(): string {
    return 'ARGUMENT';
  }
}
