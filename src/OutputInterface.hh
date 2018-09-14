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
 * This interface provides a mechanism for providing output from the CLI.
 *
 * By default, HH-CLILib provides implemenentations connected to stdout and
 * stderr; these can be replaced with other implementations, for example, mocks
 * for testing.
 *
 * @see CLIBase
 */
interface OutputInterface {
  /**
   * Write a string to output.
   *
   * @return the number of bytes written.
   */
  public function rawWrite(string $_): int;

  /**
   * DEPRECATED: Write a string to output.
   *
   * Use `writeAsync` or `rawWrite` instead.
   *
   * @return the number of bytes written.
   */
  public function write(string $_): int;

  /**
   * Fully write a string to otuput.
   */
  public function writeAsync(string $_): Awaitable<void>;

  /**
   * Returns `true` if we have hit the end of the file for the stream.
   */
  public function isEof(): bool;
}
