/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;
use namespace HH\Lib\IO;

/** Interface encapsulating the terminal.
 *
 * See the `Terminal` class for a concrete implementation. In most cases,
 * these will be directly invoked on a `CLIBase`, or on the results of
 * `$cli->getTerminal()`.
 */
interface ITerminal {
  /**
   * Determines whether your current terminal supports colors.
   *
   * This function will automatically return `true` in some non-interactive
   * cases, such as non-interactive contexts provided in continuous integration
   * (CI) systems.
   */
  public function supportsColors(): bool;

  /**
   * Determines whether the current terminal is in interactive mode.
   *
   * In general, this is `true` if the user is directly typing into stdin.
   */
  public function isInteractive(): bool;

  /**
   * Gets the standard input for the current CLI.
   *
   * By default, this is the process standard input, or file descriptor 0.
   */
  public function getStdin(): IO\ReadHandle;

  /**
   * Gets the standard process output for the current CLI.
   *
   * By default, this is the process standard output, or file descriptor 1.
   */
  public function getStdout(): IO\WriteHandle;

  /**
   * Gets the standard error output for the current CLI.
   *
   * By default, this is the process standard error, or file descriptor 2.
   */
  public function getStderr(): IO\WriteHandle;

}
