#pragma once

#include "nizer/nz.hpp"
#include <string>

namespace nz {

class error {
public:
  std::string err;
  std::string reason;
  srcref ref;

  error(std::string err, std::string reason, srcref ref)
      : err(err), reason(reason), ref(ref) {}

  error(std::string reason, srcref ref)
      : err("Unexpected error"), reason(reason), ref(ref) {}

  operator std::string();
};

} // namespace nz
