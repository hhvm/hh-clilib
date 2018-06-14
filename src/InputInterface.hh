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

/**
 * This interface provides a mechanism for reading input from the CLI.
 *
 * By default, hh-clilib provides an `InputInterface` connected to stdin. This
 * can be replaced - for example, with a mock implementation for testing.
 *
 * @see CLIBase
 */
interface InputInterface {
  /**
   * Read until we reach `$max_bytes` or the end of the file.
   *
   * @param $max_bytes must be null or >= 0
   */
  public function readAsync(?int $max_bytes = null): Awaitable<string>;

  /**
   * Read until we reach a newline, the end of the file, or `$max_bytes` bytes.
   *
   * @param $max_bytes must be null or >= 0.
   */
  public function readLineAsync(?int $max_bytes = null): Awaitable<string>;

  /**
   * Returns `true` if we have hit the end of the file for the stream.
   */
  public function isEof(): bool;
}
