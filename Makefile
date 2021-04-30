test1:
	./bin/interpreter < tests/test1.txt
test2:
	./bin/interpreter < tests/test2.txt
test3:
	./bin/interpreter < tests/test3.txt
rm:
	rm bin/interpreter
interpreter:
	g++ src/* -o bin/interpreter -I includes/ -std=c++11
