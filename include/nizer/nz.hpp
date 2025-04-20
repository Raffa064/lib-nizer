#pragma once

#include <nizer/parser.hpp>
#include <nizer/sym.hpp>
#include <nizer/token.hpp>
#include <nizer/visitor.hpp>
#include <string>

namespace nz {

token_vector tokenize(SymbolList symbols, std::string &);

}; // namespace nz
