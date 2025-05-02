#pragma once

#include <any>
#include <initializer_list>
#include <string>

namespace nz {

using ils = std::initializer_list<std::any>;

inline std::string any_to_string(const std::any &value) {
  if (value.type() == typeid(int)) {
    return std::to_string(std::any_cast<int>(value));
  } else if (value.type() == typeid(double)) {
    return std::to_string(std::any_cast<double>(value));
  } else if (value.type() == typeid(std::string)) {
    return std::any_cast<std::string>(value);
  } else if (value.type() == typeid(const char *)) {
    return std::string(std::any_cast<const char *>(value));
  }

  return "<unprintable-type>";
}

class error {
public:
  std::string msg;

  error(std::string msg, ils insertion = {}) {
    int index = 0;
    for (auto ins : insertion) {
      index = msg.find("{}", index);

      std::string str = any_to_string(ins);
      msg = msg.replace(index, 2, str);
      index += str.length();
    }

    this->msg = msg;
  }

  operator std::string() { return msg; }
  operator const char *() { return msg.c_str(); }
};

} // namespace nz
