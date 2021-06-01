# INTERPRETER
This program is an interpreter of author programming language.

## Content
* [Definition](#basic)
* [Operations](#operations)
* [How to download](#download)
* [Compiling](#compile)
* [Tests](#test)

# <a name="basic"></a> Definition

**An interpreter** is a computer program that directly executes instructions written in a programming or scripting language, without requiring them previously to have been compiled into a machine language program.

# <a name="operations"></a> Operations

1) Program supports following operators:
    
    | OPERATOR | SIGN |
    | ----------- | ----------- |
    | Assign | := |
    | Logical OR | | |
    | Logical AND | & |
    | Logical XOR | ^ |
    | Equal | == |
    | Not equal | != |
    | Greater or equal | >= |
    | Greater | > |
    | Less or equal | <= |
    | Less | < |
    | Plus | + |
    | Minus | - |
    | Multiply | * |
    | Integer division | / |
    | Integer division remainder | % |
    
2) Program supports jumps (goto, L:)

3) Program supports condition (if, else, @)

4) Program supports loop (while, @)

5) Program supports functions:
a. without arguments
```
def foo()
    ...
fed
```
b. with 1 argument
```
def foo(arg)
    ...
fed
```
c. with more than 1 arguments
```
def foo(arg1, arg2, arg3)
    ...
fed
```

# <a name="download"></a> How to download
  ```
  `git clone https://github.com/Anastasiya574/interpreter`
  ```
  
# <a name="compile"></a> Compiling
```
make interpreter
```

# <a name="test"></a> Tests
```
make test1
make test2
make test3
make test4
```
