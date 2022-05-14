// =========================================================================
// Copyright 2021 -- present Liam Huang (Yuuki) [liamhuang0205@gmail.com].
// Author: Liam Huang
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =========================================================================

#include "jsondag/helper/file.h"

#include <catch.h>

#include <cstdio>

TEST_CASE("File") {
  jsondag::helper::File f("./test/cases/basic.json", "r");
  REQUIRE_FALSE(nullptr == f);        // derived from std::unique_ptr<std::FILE, DELETER>
  REQUIRE_FALSE(nullptr == f());      // raw std::FILE*
  REQUIRE_FALSE(nullptr == f.get());  // raw std::FILE*
}

TEST_CASE("File Reading") {
  jsondag::helper::File f("./test/cases/basic.json", "r");
  REQUIRE_FALSE(nullptr == f);
  int c;
  while ((c = std::fgetc(f())) != EOF && c == ' ' && c == '\n' && c == '\t') {
  }
  REQUIRE(c == '[');  // first valid char should be left bracket.
}
