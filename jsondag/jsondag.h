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
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "jsondag/helper/file.h"
#include "jsondag/node.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

namespace jsondag {
class JsonDag {
 private:
  std::string json_fname_;
  std::vector<Node> nodes_;
  std::unordered_map<std::string, size_t> node_map_;
  std::vector<size_t> topological_sorted_;

 public:
  JsonDag() = default;
  explicit JsonDag(const std::string& fname) : json_fname_(fname) {
  }
  ~JsonDag() = default;

 public:
  void json_fname(const std::string& fname) {
    json_fname_ = fname;
  }

  bool parse() {
    if (json_fname_.empty()) {
      return false;
    }

    helper::File f(json_fname_.c_str(), "r");
    char buffer[65536];
    rapidjson::FileReadStream is(f(), buffer, sizeof(buffer));
    rapidjson::Document d;
    d.ParseStream(is);
    if (!d.IsArray()) {
      return false;
    }

    return _build_nodes(d) && _build_successors() && _topological_sort();
  }

 private:
  bool _build_nodes(rapidjson::Document& d) {
    uint64_t id = 0UL;
    nodes_.reserve(d.Size());
    for (auto& obj : d.GetArray()) {
      nodes_.emplace_back(id, obj);
      if (!nodes_.back().parse()) {  // doesn't even have a name.
        return false;
      }
      auto [_, succ] = node_map_.emplace(nodes_.back().name(), id);
      if (!succ) {  // different nodes share the same name.
        return false;
      }
      ++id;
    }
    return true;
  }

  bool _build_successors() {
    for (auto& node : nodes_) {
      if (!node.valid()) {
        return false;
      }
      const auto& predecessors = node.predecessors();
      for (const auto& pred : predecessors) {
        auto iter = node_map_.find(pred);
        if (node_map_.end() == iter) {  // failed to find predecessor.
          return false;
        }
        auto& pred_node = nodes_[iter->second];
        pred_node.mutable_successors()->emplace_back(node.name());
      }
    }
    return true;
  }

  bool _topological_sort() {  // greedy sort
    std::vector<uint64_t> in_degree;
    in_degree.reserve(nodes_.size());
    for (auto& node : nodes_) {
      in_degree.emplace_back(node.in_degree());
    }

    topological_sorted_.clear();
    topological_sorted_.reserve(nodes_.size());

    std::stack<size_t> s;

    for (size_t i = 0; i != in_degree.size(); ++i) {
      if (in_degree[i] == 0) {
        s.push(i);
      }
    }

    while (!s.empty()) {
      topological_sorted_.emplace_back(s.top());
      s.pop();

      for (const auto& succ : nodes_[topological_sorted_.back()].successors()) {
        auto iter = node_map_.find(succ);
        if (node_map_.end() == iter) {
          return false;
        }
        in_degree[iter->second]--;
        if (0 == in_degree[iter->second]) {
          s.push(iter->second);
        }
      }
    }

    return topological_sorted_.size() == nodes_.size();  // acyclic test
  }

 public:
  bool valid() const {
    return !nodes_.empty() && nodes_.size() == node_map_.size() &&
           topological_sorted_.size() == nodes_.size();
  }

  const std::string& json_fname() const {
    return json_fname_;
  }
};
}  // namespace jsondag
