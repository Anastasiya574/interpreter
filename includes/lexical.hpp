#ifndef LEXICAL_HPP
#define LEXICAL_HPP

#include <vector>
#include <string>
#include <map>
#include "lexem.hpp"

Lexem *is_oper(std::string codeline, int &i, int size);

Lexem *is_number(std::string codeline, int &i, int size);

Lexem *is_variable(std::string codeline, int &i, int size);

std::vector<Lexem *> parse_lexem(std::string codeline);

#endif
