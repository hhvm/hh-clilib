/*
 *  Copyright (c) 2017-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the MIT license found in the
 *  LICENSE file in the root directory of this source tree.
 *
 */

namespace Facebook\CLILib;

class TestCLIWithOptionalArguments extends CLIWithArguments {
  use TestCLITrait;

  public function getArgumentsForTesting(): vec<string> {
    return $this->getArguments();
  }
}
