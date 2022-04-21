#include "Expression.h"

#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC  
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

using namespace std;

int main(int argc, char* argv[]) {
	VS_MEM_CHECK

	//open files
	if (argc < 3) {
		cerr << "Please provide name of input and output files";
		return 1;
	}
	ifstream in(argv[1]);
	if (!in.is_open()) {
		cout << "Uh oh there was an inputFile error..." << endl;
		return 2;
	}
	ofstream out(argv[2]);
	if (!out.is_open()) {
		cout << "Uh oh there was an outputFile error..." << endl;
		return 3;
	}

	string inputExp;
	Expression expression;
	bool postFixCalled = false;

	//process input strings
	string line, command;
	while (getline(in, line)) {
		try {
			if (line.size() == 0) {
				continue;
			}
			stringstream iss(line);
			iss >> command;
			//if receiving a new input, reset expression values and print the original expression
			if (command == "Expression:") {
				out << endl;
				getline(iss, inputExp);
				expression.setExpression(inputExp);
				expression.clear();
				postFixCalled = false;
				out << "Expression:" << expression << endl;
			}
			//otherwise follow given commands
			else if (command == "Infix:") {
				out << "Infix: " << expression.infix() << endl;
			}
			else if (command == "Postfix:") {
				out << "Postfix: " << expression.postfix() << endl;
				postFixCalled = true;
			}
			else if (command == "Prefix:") {
				if (!postFixCalled) {
					expression.postfix();
				}
				out << "Prefix: " << expression.prefix() << endl;
			}
			else if (command == "Value:") {
				if (!postFixCalled) {
					expression.postfix();
				}
				out << "Value: " << expression.value() << endl;
			}
		}
		catch (exception& e) {
			out << e.what() << endl;
		}
	}

	in.close();
	out.close();
	return 0;
}