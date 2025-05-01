#ifndef SYMBOLS_HPP
#define SYMBOLS_HPP

#include "Pascal--/lexical/TokenType.hpp"

namespace SymbolTable {

bool contains(const std::string& token);

enum TokenType find(const std::string& token);

} // namespace SymbolTable

#endif // SYMBOLS_HPP
