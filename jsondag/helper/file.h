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
  File(const char* fname, const char* mode) : __wrapper(std::fopen(fname, mode), __close_file) {
  }

  std::FILE* operator()() const noexcept {
    return get();
  }
};
}  // namespace helper
}  // namespace jsondag
