#include <nizer/err.hpp>

namespace nz {

error::operator std::string() {
  // Ex: Unexpected error at 2:34 'fn:void': Invalid token
  return err + " at " + ((std::string)ref) + ": " + reason;
}

} // namespace nz
