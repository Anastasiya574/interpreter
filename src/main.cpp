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
 /*
void printTable() {
    cout << "-----------------TABLE---------------" << endl;
    for (map<string, int>::iterator i = vtable.begin(); i != vtable.end(); i++) {
        cout << i->first << ' ' << i->second << ' ' << endl;
    }
    cout << "-------------------------------------" << endl;
*/
int main(int argc, char** argv) {
    string codeline;
    vector<vector<Lexem *>> infixlines, postfixlines;
    try {
        while (getline(cin, codeline)) {
            try {
                infixlines.push_back(parse_lexem(codeline));
            } catch (ERRORS e) {
                cout << "problems in " << infixlines.size() + 1 << " string:\n";
                throw (e);
            }
        }
        init_loops(infixlines);
        for (int row = 0; row < infixlines.size(); ++row) {
            try {
                init_labels(infixlines[row], row);
            } catch (ERRORS e) {
                cerr << "problems in " << row + 1 << " string:\n";
                throw (e);
            }
        }
        init_functions(infixlines);
        for (const auto &infix: infixlines) {
            try {
                postfixlines.push_back(build_postfix(infix));
            } catch (ERRORS e) {
                cerr << "problems in " << postfixlines.size() + 1 << " string:\n";
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
                } catch (ERRORS e) {
                    cerr << "problems in " << row + 1 << " string:\n";
                    throw (e);	
                }
            }
    } catch (ERRORS e) {
        cerr << ERRORTEXT[int(e)] << endl;
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
