#pragma once

#include <nizer/parser.hpp>
#include <nizer/sym.hpp>
#include <nizer/token.hpp>
#include <nizer/visitor.hpp>
#include <string>

namespace nz {

token_vector tokenize(SymbolList symbols, std::string &);

std::string at(std::string source, int index);

}; // namespace nz
