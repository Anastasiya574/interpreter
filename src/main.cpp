#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stack>
#include <map>
#include <set>
#include <cctype>
#include "const.hpp"
#include "lexem.hpp"
#include "lexical.hpp"
#include "semantic.hpp"
#include "syntax.hpp"

int main(int argc, char** argv) {
	for (int i = 1, flag = 0; i < argc; i++) {
		if (strstr(argv[i], ".txt") != nullptr) {
			if (strcmp(argv[i - 1], "<") == 0 || strcmp(argv[i - 1], "<<") == 0) 
				INTERACTIVE = false;
		}
	}
	std::string codeline;
	std::vector<std::vector<Lexem *>> infixlines, postfixlines;
	try {
		while (getline(std::cin, codeline)) {
			try {
				infixlines.push_back(parse_lexem(codeline));
			} catch (ERRORS e) {
				std::cout << "problems in " << infixlines.size() + 1 << " string:\n";
				throw (e);
			}
		}
		std::cout << "before initloops" << std::endl;
		init_loops(infixlines);
		std::cout << "after initloops" << std::endl;
		for (int row = 0; row < infixlines.size(); ++row) {
			try {
				init_labels(infixlines[row], row);
			} catch (ERRORS e) {
				std::cerr << "problems in " << row + 1 << " string:\n";
				throw (e);
			}
		}
		init_functions(infixlines);
		for (const auto &infix: infixlines) {
			try {
				postfixlines.push_back(build_postfix(infix));
			} catch (ERRORS e) {
				std::cerr << "problems in " << postfixlines.size() + 1 << " string:\n";
				throw (e);	
			}
		}
		Number *res = nullptr;
		if (Function::ftable.find("main") == Function::ftable.end()) {
            throw (ERR_UNDEFINED_MAIN);
        }
        int row = Function::ftable["main"];
        Function *main = new Function("main", row, 0);
		while (0 <= row && row < (int)postfixlines.size()) {
			try {
				row = evaluate_postfix(postfixlines, row, &main, &res);
				std::cout << std::endl;
			} catch (ERRORS e) {
				std::cerr << "problems in " << row + 1 << " string:\n";
				throw (e);	
			}
		}
	} catch (ERRORS e) {
		std::cerr << ERRORTEXT[int(e)] << std::endl;
	}
	for (int i = 0; i < infixlines.size(); i++) {
		for (auto &cl: infixlines[i]) {
			if (cl) {
				delete cl;
			}
		}
	}
	return 0;
}
