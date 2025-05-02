#include <nizer/err.hpp>

namespace nz {

error::operator std::string() {
  // Ex: Unexpected error at 2:34 'fn:void': Invalid token
  return err + " at \e[32m" + ref.source + "\e[0m: " + reason;
}

} // namespace nz
