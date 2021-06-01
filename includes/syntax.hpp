#ifndef SYNTAX_HPP
#define SYNTAX_HPP

#include <vector>
#include <string>
#include <map>
#include "lexem.hpp"

std::vector<Lexem *> build_postfix(std::vector<Lexem *> infix);

#endif
