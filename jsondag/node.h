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
#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace jsondag {
class Node {
 private:
  uint64_t id_ = std::numeric_limits<uint64_t>::max();
  rapidjson::Value json_val_;
  std::string name_;
  std::vector<std::string> predecessors_;
  std::vector<std::string> successors_;
  std::string json_str_;

 public:
  Node() = delete;
  Node(uint64_t id, rapidjson::Value& json_val) : id_(id) {
    json_val_ = json_val;
  }

 public:
  bool parse() {
    auto name_itr = json_val_.FindMember("name");
    if (json_val_.MemberEnd() == name_itr || !name_itr->value.IsString()) {
      return false;
    }
    name_ = name_itr->value.GetString();

    auto deps_itr = json_val_.FindMember("deps");
    if (json_val_.MemberEnd() != deps_itr && deps_itr->value.IsArray()) {
      predecessors_.reserve(deps_itr->value.Size());
      for (auto& ele : deps_itr->value.GetArray()) {
        if (ele.IsString()) {
          predecessors_.emplace_back(ele.GetString());
        }
      }
    }

    rapidjson::StringBuffer buff;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    json_val_.Accept(writer);
    json_str_ = buff.GetString();

    return true;
  }

 public:
  bool valid() const {
    return id_ != std::numeric_limits<uint64_t>::max();
  }

  uint64_t id() const {
    return id_;
  }

  const std::string& name() const {
    return name_;
  }

  uint64_t in_order() const {
    return predecessors_.size();
  }

  uint64_t out_order() const {
    return successors_.size();
  }

  const std::string& json_str() const {
    return json_str_;
  }
};
}  // namespace jsondag
