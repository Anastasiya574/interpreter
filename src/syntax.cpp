#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <set>
#include <cctype>

#include "const.hpp"
#include "lexem.hpp"
#include "lexical.hpp"
#include "semantic.hpp"
#include "syntax.hpp"

std::vector<Lexem *> build_postfix(std::vector<Lexem *> infix) {
	std::vector<Lexem *> lexem_stack;
	std::vector<Oper *> operator_stack;
	for (auto &element: infix) {
		if (element == nullptr) {
			continue;
		}
		if (Number *ptr = dynamic_cast<Number *>(element)) {
			lexem_stack.push_back(element);
			continue;
		}
		if (Variable *ptr = dynamic_cast<Variable *>(element)) {
			lexem_stack.push_back(element);
			continue;
		}
		OPERATOR operatortype = ((Oper *)element) -> get_type();
		if (operator_stack.empty() || operatortype == LBRACKET) {
			operator_stack.push_back((Oper *)element);
			continue;
		}
		if (operatortype == RBRACKET) {
			if (operator_stack.empty()) {
				throw (ERR_NOT_BALANCED_BRACKETS);
			}
			while ((operator_stack.back()) -> get_type() != LBRACKET ) {
				lexem_stack.push_back(operator_stack.back());
				operator_stack.pop_back();
				if (operator_stack.empty()) {
					throw (ERR_NOT_BALANCED_BRACKETS);
				}
			}
			operator_stack.pop_back();
			if (!operator_stack.empty() &&(operator_stack.back()) -> get_type() == FUNCTION) {
				lexem_stack.push_back(operator_stack.back());
				operator_stack.pop_back();
			}
			continue;
		} else if(operatortype == RSQRBRACKET) {
        while(!operator_stack.empty() && (operator_stack.back()->get_type() != LSQRBRACKET)) {
            lexem_stack.push_back(operator_stack.back());
            operator_stack.pop_back();  
        }
        lexem_stack.push_back(operator_stack.back());
        operator_stack.pop_back();  
    }
		if (operatortype == COMMA) {
			while ((operator_stack.back()) -> get_type() != LBRACKET) {
				lexem_stack.push_back(operator_stack.back());
				operator_stack.pop_back();
			}
			continue;
		}
		if (operatortype == ASSIGN && ASSIGN == (operator_stack.back()) -> get_type()) {
			operator_stack.push_back((Oper *)element);
			continue;
		}
		if (operatortype != FUNCTION && ((Oper *)element) -> get_priority() <= (operator_stack.back()) -> get_priority()) {
			while (!(operator_stack.empty()) && ((operator_stack.back()) -> get_priority() > 0)) {
				lexem_stack.push_back(operator_stack.back());
				operator_stack.pop_back();
			}
			operator_stack.push_back((Oper *)element);
		} else {
			operator_stack.push_back((Oper *)element);
		}
	}
	while (!operator_stack.empty()) {
		lexem_stack.push_back(operator_stack.back());
		operator_stack.pop_back();
	}
	return lexem_stack;
}
