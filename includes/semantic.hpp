#ifndef SEMANTIC_HPP
#define SEMANTIC_HPP

#include <vector>
#include <string>
#include <map>
#include "lexem.hpp"

void init_labels(std::vector<Lexem *> &infix, int row);

void init_loops(std::vector<std::vector<Lexem *>> &infix);

void init_function_start_vars(std::vector<Lexem *> &infix, int &i, std::map<std::string, std::set<std::string>> &func_var_table);

void init_functions(std::vector<std::vector<Lexem *>> &infix);

int evaluate_postfix(std::vector<std::vector<Lexem *>> &postfix, int row, Function **function_field, Number **res);

#endif
