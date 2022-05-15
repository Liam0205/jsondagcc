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

#include <cstdio>
#include <functional>
#include <memory>

namespace jsondag {
namespace helper {
void __close_file(std::FILE* fp) {
  std::fclose(fp);
}

using __wrapper = std::unique_ptr<std::FILE, std::function<void(std::FILE*)>>;

struct File : public __wrapper {
  File(const char* fname, const char* mode)
      : __wrapper(std::fopen(fname, mode), __close_file) {
  }

  std::FILE* operator()() const noexcept {
    return get();
  }
};
}  // namespace helper
}  // namespace jsondag
