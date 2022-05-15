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

#include "jsondag/jsondag.h"

#include <catch.h>

#include <algorithm>
#include <iterator>
#include <sstream>

TEST_CASE("jsondag.basic") {
  const char* basic_json = "./test/cases/basic.json";
  jsondag::JsonDag jd(basic_json);
  REQUIRE_FALSE(jd.valid());
  REQUIRE(basic_json == jd.json_fname());
  REQUIRE(jd.parse());
  REQUIRE(jd.valid());

  const auto& nodes = jd.nodes();
  for (auto& node : nodes) {
    REQUIRE(node.valid());
    if (node.name() == "A") {
      REQUIRE(node.id() == 0);
      REQUIRE(node.in_degree() == 0);
      REQUIRE(node.out_degree() == 1);
    } else if (node.name() == "B") {
      REQUIRE(node.id() == 1);
      REQUIRE(node.in_degree() == 2);
      REQUIRE(node.out_degree() == 0);
    } else if (node.name() == "C") {
      REQUIRE(node.id() == 2);
      REQUIRE(node.in_degree() == 0);
      REQUIRE(node.out_degree() == 1);
    } else {
      REQUIRE(false);
    }
  }

  const std::vector<size_t>& sorted = jd.topological_sorted();
  std::ostringstream oss;
  std::transform(sorted.begin(), sorted.end(),
                 std::ostream_iterator<std::string>(oss, ","),
                 [&nodes](size_t i) { return nodes[i].name(); });
  REQUIRE(oss.str() == "C,A,B,");
}
