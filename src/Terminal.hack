/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

use namespace HH\Lib\{IO, Str};

/**
 * Default implementation of an `ITerminal`.
 *
 * An instance can be retrieved in a CLI class via `$this->getTerminal()`.
 */
final class Terminal implements ITerminal {
  public function __construct(
    private IO\ReadHandle $stdin,
    private IO\WriteHandle $stdout,
    private IO\WriteHandle $stderr,
  ) {
  }

  <<__Memoize>>
  public function supportsColors(): bool {
    $colors = \getenv('CLICOLORS');
    if (
      $colors === '1' ||
      $colors === 'yes' ||
      $colors === 'true' ||
      $colors === 'on'
    ) {
      return true;
    }
    if (
      $colors === '0' ||
      $colors === 'no' ||
      $colors === 'false' ||
      $colors === 'off'
    ) {
      return false;
    }

    if (\getenv('TRAVIS') === 'true') {
      return true;
    }

    if (\getenv('CIRCLECI') === 'true') {
      return true;
    }

    if (\getenv('TERM') === 'xterm') {
      return true;
    }

    if (\getenv('TERM_PROGRAM') === 'Hyper') {
      return true;
    }

    if ($this->isInteractive()) {
      return Str\contains((string)\getenv('TERM'), 'color');
    }

    return false;
  }

  /**
   * Determines whether the current terminal is in interactive mode.
   *
   * In general, this is `true` if the user is directly typing into stdin.
   */
  <<__Memoize>>
  public function isInteractive(): bool {
    // Explicit override
    $noninteractive = \getenv('NONINTERACTIVE');
    if ($noninteractive !== false) {
      if ($noninteractive === '1' || $noninteractive === 'true') {
        return false;
      }
      if ($noninteractive === '0' || $noninteractive === 'false') {
        return true;
      }
      /* HHAST_IGNORE_ERROR[DontUseAsioJoin] */
      \HH\Asio\join($this->stderr->writeAsync(
        "NONINTERACTIVE env var must be 0/1/yes/no/true/false, or unset.\n",
      ));
    }

    // Detects TravisCI and CircleCI; Travis gives you a TTY for STDIN
    $ci = \getenv('CI');
    if ($ci === '1' || $ci === 'true') {
      return false;
    }

    // Generic
    if (\posix_isatty(\STDIN) && \posix_isatty(\STDOUT)) {
      return true;
    }

    // Fail-safe
    return false;
  }

  /**
   * Gets the input interface for the current CLI.
   *
   * By default, this is the process standard input, or file descriptor 0.
   */
  public function getStdin(): IO\ReadHandle {
    return $this->stdin;
  }

  /**
   * Gets the output interface for the current CLI.
   *
   * By default, this is the process standard output, or file descriptor 1.
   */
  public function getStdout(): IO\WriteHandle {
    return $this->stdout;
  }

  /**
   * Gets the error interface for the current CLI.
   *
   * By default, this is the process standard error, or file descriptor 2.
   *
   * This is usually a wrapper around stdout, and should be used instead of
   * direct access.
   */
  public function getStderr(): IO\WriteHandle {
    return $this->stderr;
  }
}
