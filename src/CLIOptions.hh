<?hh
/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib\CLIOptions;

function flag(
  (function():void) $setter,
  string $help_text,
  string $long,
  ?string $short = null,
): CLIOption {
  return new CLIOptionFlag($setter, $help_text, $long, $short);
}

function with_required_string(
  (function(string):void) $setter,
  string $help_text,
  string $long,
  ?string $short = null,
): CLIOption {
  return new CLIOptionWithRequiredStringValue($setter, $help_text, $long, $short);
}

function with_required_enum<T>(
  enumname<T> $enum,
  (function(T):void) $setter,
  string $help_text,
  string $long,
  ?string $short = null,
): CLIOption {
  return new CLIOptionWithRequiredEnumValue($enum, $setter, $help_text, $long, $short);
}
