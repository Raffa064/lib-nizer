#include <regex>
#include <sym.h>

bool SymMatch::operator()(Token current) {
  if (typeId != current.typeId) {
    return false;
  }

  std::regex regex(match);
  if (!std::regex_match(current.value, regex))
    return false;

  return true;
}

Symbol::operator SymMatch() { return SymMatch(typeId, ".*"); }

bool Symbol::operator()(Token token) { return ((SymMatch)(*this))(token); }

SymMatch Symbol::operator()(std::string match) {
  return SymMatch(typeId, match);
}
