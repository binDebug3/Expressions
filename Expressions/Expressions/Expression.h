#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ExpressionManagerInterface.h"

#include <string>
#include <stack>
#include <vector>
#include <sstream>

using namespace std;

class Expression : public ExpressionManagerInterface {
private:
	bool postOperation = true;			//is true when we are not creating the preFix expression
	string expression;					//holds the inFix expression
	stringstream storePostFix;			//stores the postFix expression in a stringstream
	string operators = "([{ -+  */% ";	//used to set precedence

	//PRIVATE MEMBER FUNCTIONS are designed to be more condense easier to read
	//returns true if the char/string is an operator-------------------------------------
	bool isOperator(char c) const {
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
			return true;
		}
		return false;
	}
	bool isOperator(string s) const {
		char c = s.at(0);
		if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
			return true;
		}
		return false;
	}
	//returns true if the char/string is a grouping character----------------------------
	bool isOpenParen(char p) const {
		if (p == '(' || p == '[' || p == '{') return true;
		return false;
	}
	bool isOpenParen(string s) const {
		if (s == "(" || s == "[" || s == "{") return true;
		return false;
	}
	bool isCloseParen(char p) const {
		if (p == ')' || p == ']' || p == '}') return true;
		return false;
	}
	bool isCloseParen(string s) const {
		if (s == ")" || s == "]" || s == "}") return true;
		return false;
	}
	bool isParen(char g) const {
		if (isOpenParen(g) || isCloseParen(g)) return true;
		return false;
	}
	bool isParen(string g) const {
		if (isOpenParen(g) || isCloseParen(g)) return true;
		return false;
	}
	//returns true if the chars are the same kind of grouping character------------------
	bool areCompliments(char open, char close) const {
		if		(close == ')') close = '(';
		else if (close == ']') close = '[';
		else if (close == '}') close = '{';
		if (open == close) {
			return true;
		}
		return false;
	}
	//flip grouping operators------------------------------------------------------------
	string flipOperator(string paren) const {
		if		(paren == ")") paren = "(";
		else if (paren == "]") paren = "[";
		else if (paren == "}") paren = "{";
		else if (paren == "(") paren = ")";
		else if (paren == "[") paren = "]";
		else if (paren == "{") paren = "}";
		return paren;
	}
	//checks reletive precedence of two operators (in Expression.cpp)--------------------
	bool validPrecedence(string token, string stackToken) const;

//+++++++++++++++ PUBLIC FUNCTIONS ++++++++++++++++++++++++++++++++++++++++++++++++++++++

public:
	Expression(string exp = "") :
		expression(exp)
	{}
	virtual ~Expression(void) {}

	//updates the expression variable
	void setExpression(string expression) {
		this->expression = expression;
	}
	//resets the stringstream data member
	void clear(void) {
		stringstream().swap(storePostFix);
	}

	virtual int value(void);		/** Return the integer value of the infix expression */

	virtual string infix(void);		/** Return the infix items from the expression */

	virtual string postfix(void);	/** Return a postfix representation of the infix expression */

	virtual string prefix(void);	/** Return a prefix representation of the infix expression */


	/** Return the infix expression item */
	virtual string toString(void) const {
		return expression;
	}
	friend ostream& operator<< (ostream& os, Maze& cube) {
		os << cube.toString();
		return os;
	}
};
#endif