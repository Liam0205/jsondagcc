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

#include "jsondag/node.h"

#include <catch.h>

#include <cstdio>
#include <iostream>
#include <vector>

#include "jsondag/helper/file.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"

TEST_CASE("node.basic") {
  jsondag::helper::File f("./test/cases/basic.json", "r");
  char buffer[65536];
  rapidjson::FileReadStream is(f(), buffer, sizeof(buffer));
  rapidjson::Document d;
  d.ParseStream(is);
  REQUIRE(d.IsArray());
  uint64_t id = 0UL;
  for (auto& obj : d.GetArray()) {
    REQUIRE(obj.IsObject());
    jsondag::Node node(id++, obj);
    REQUIRE(node.parse());
    REQUIRE(node.valid());
    if (node.name() == "A") {
      REQUIRE(node.id() == 0);
      REQUIRE(node.in_degree() == 0);
      jsondag::Node new_node;
      new_node = std::move(node);
      REQUIRE(new_node.valid());
      REQUIRE_FALSE(node.valid());
      REQUIRE(new_node.id() == 0);
      REQUIRE(new_node.in_degree() == 0);
    } else if (node.name() == "B") {
      REQUIRE(node.id() == 1);
      REQUIRE(node.in_degree() == 2);
      jsondag::Node new_node(std::move(node));
      REQUIRE(new_node.valid());
      REQUIRE_FALSE(node.valid());
      REQUIRE(new_node.id() == 1);
      REQUIRE(new_node.in_degree() == 2);
    } else if (node.name() == "C") {
      REQUIRE(node.id() == 2);
      REQUIRE(node.in_degree() == 0);
    } else {
      REQUIRE(false);
    }
  }
}
