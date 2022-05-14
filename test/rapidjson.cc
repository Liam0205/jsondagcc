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

#include "rapidjson/rapidjson.h"

#include <catch.h>

#include <cstdio>
#include <iostream>
#include <vector>

#include "jsondag/helper/file.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

TEST_CASE("rapidjson.DOM-manipulate") {
  jsondag::helper::File f("./test/cases/basic.json", "r");
  char buffer[65536];
  rapidjson::FileReadStream is(f(), buffer, sizeof(buffer));
  rapidjson::Document d;
  d.ParseStream(is);
  REQUIRE(d.IsArray());
  for (auto& obj : d.GetArray()) {
    REQUIRE(obj.IsObject());
    rapidjson::Value::ConstMemberIterator itr = obj.FindMember("name");
    REQUIRE(obj.MemberEnd() != itr);
    REQUIRE(itr->value.IsString());

    itr = obj.FindMember("deps");
    if (obj.MemberEnd() != itr) {
      REQUIRE(itr->value.IsArray());
      for (auto& ele : itr->value.GetArray()) {
        REQUIRE(ele.IsString());
      }
    }

    // rapidjson::StringBuffer buff;
    // rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    // obj.Accept(writer);
    // std::cout << buff.GetString() << std::endl;
  }
}

TEST_CASE("rapidjson.DOM-store in vector") {
  jsondag::helper::File f("./test/cases/basic.json", "r");
  char buffer[65536];
  rapidjson::FileReadStream is(f(), buffer, sizeof(buffer));
  rapidjson::Document d;
  d.ParseStream(is);
  REQUIRE(d.IsArray());
  std::vector<rapidjson::Value> vals;
  vals.resize(d.Size());
  int i = 0;
  for (auto& obj : d.GetArray()) {
    REQUIRE(obj.IsObject());
    vals[i].CopyFrom(obj, d.GetAllocator());
    REQUIRE(vals[i++] == obj);
  }
}
