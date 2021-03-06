#include <string>
#include "const.hpp"

int PRIORITY[] = { 
    -5,
    -6, 0, 0, 
    -1, -1, -4, 
    -3, -3,
    -3, -3,
    -3, -3,   
    -2, -2, -2,
    -1, -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6, 6,
    7, 7,
    7, 7,
    8, 8,
    9, 9,
    10, 10, 10,
};

std::string OPERTEXT[] = {
    ",",
    "return",
    "def", "fed",
    "[", "]", "size",
    "if", "then",
    "else", "endif",
    "while", "@", "endwhile",
    "goto", ":",
    "(", ")",
    "=",
    "or",
    "and",
    "|",
    "^",
    "&",
    "==", "!=",
    "<=", "<",
    ">=", ">",
    "<<", ">>",
    "+", "-",
    "*", "/", "%",
};

bool DEBUG = false;

bool INTERACTIVE = true;

int OPER_OR_VAR[] = {1, 2, 3, 6, 7, 8, 9, 10, 11, 13, 14, 19, 20, 38, 39};

int OPER_NUMBER = sizeof(PRIORITY) / sizeof(int);

