#include "Expression.h"

#include <string>
#include <stack>
#include <vector>
#include <sstream>

using namespace std;

//--------------------------------VALUE------------------------------------------------------------
int Expression::value(void) {
	stack<int> valueStack;
	string token;
	int operandR = 0, operandL = 0, pushOperand = 0;

	while (storePostFix >> token) {
		//push the token onto valueStack if it is a digit
		if (isdigit(token.at(0))) {
			int numToken = stoi(token);
			valueStack.push(numToken);
		}
		//otherwise perform the appropriate operation
		else {
			operandL = valueStack.top();
			valueStack.pop();
			operandR = valueStack.top();
			valueStack.pop();
			if (token.at(0) == '%') {
				pushOperand = operandR % operandL;
			}
			else if (token.at(0) == '/') {
				pushOperand = operandR / operandL;
			}
			else if (token.at(0) == '*') {
				pushOperand = operandR * operandL;
			}
			else if (token.at(0) == '+') {
				pushOperand = operandR + operandL;
			}
			else if (token.at(0) == '-') {
				pushOperand = operandR - operandL;
			}
			//then push calculated value into the stack
			valueStack.push(pushOperand);
		}
	}
	return valueStack.top();
}
//--------------------------------INFIX------------------------------------------------------------
string Expression::infix(void) {
	//check for expression
	if (expression == "") {
		return "Invalid Expression";
	}
	//check if expression is balanced by comparing all grouping operators
	stack<char> parenStack;
	for (unsigned int i = 0; i < expression.length(); i++) {
		char paren = expression.at(i);
		if (isOpenParen(paren)) {
			parenStack.push(paren);
		}
		else if (isCloseParen(paren) && !parenStack.empty()) {
			char tempToken = parenStack.top();
			parenStack.pop();
			if (!areCompliments(tempToken, paren)) {
				return "Unbalanced";
			}
		}
	}
	//if this algorithm doesn't end with an empty stack, it is unbalanced
	if (!parenStack.empty()) {
		return "Unbalanced";
	}
	//check beginning for missing operands
	if (!isspace(expression.at(1)) && !isdigit(expression.at(1)) && !isParen(expression.at(1))) {
		return "Missing Operand";
	}
	for (unsigned int i = 0; i < expression.length() - 2; i++) {
		char token1 = expression.at(i);
		char token2 = expression.at(i + 1);
		char token3 = expression.at(i + 2);
		//check for missing operands by searching for two operands in a row
		if (isOperator(token1) && isspace(token2) && isOperator(token3)) {
			return "Missing Operand";
		}
		//check for missing operator by searching for two math operators in a row
		if (isdigit(token1) && isspace(token2) && isdigit(token3)) {
			return "Missing Operator";
		}
	}
	//check end for missing operands
	char checkEnd = expression.at(expression.length() - 1);
	if (!isdigit(checkEnd) && !isParen(checkEnd)) {
		return "Missing Operand";
	}
	//check for illegel operators by comparing token to all correct possible input
	for (unsigned int i = 0; i < expression.length(); i++) {
		char token = expression.at(i);
		if (isalpha(token) || !isspace(token) &&
			!(isOperator(token) || isdigit(token) || isParen(token))) {
			return "Illegal Operator";
		}
	}
	//return infix expression
	return expression;
}
//--------------------------------POSTFIX----------------------------------------------------------
string Expression::postfix(void) {
	string postFixString;
	stack<string> postStack;
	bool success = true;
	stringstream iss(expression);
	string token;
	string tempToken;

	while (success && iss >> token) {
		//add token to postFixString if its a digit
		if (isdigit(token.at(0))) {
			postFixString += token + " ";
			storePostFix << token << " ";
		}
		//push token if its an open parenthese
		else if (isOpenParen(token)) {
			postStack.push(token);
		}
		//pop accordingly if token is a close parenthese
		else if (isCloseParen(token)) {
			tempToken = postStack.top();
			do {
				tempToken = postStack.top();
				postStack.pop();
				if (isOperator(tempToken)) {
					postFixString += tempToken + " ";
					storePostFix << tempToken << " ";
				}
			} while (!postStack.empty() && !isOpenParen(tempToken));
		}
		//pop accordingly if token is an operator
		else if (isOperator(token)) {
			while (!postStack.empty() && validPrecedence(token, postStack.top())) {
				postFixString += postStack.top() + " ";
				storePostFix << postStack.top() << " ";
				postStack.pop();
			}
			postStack.push(token);
		}
		else {
			success = false;
		}
	}
	//empty postStack
	while (!postStack.empty()) {
		postFixString += postStack.top() + " ";
		storePostFix << postStack.top() << " ";
		postStack.pop();
	}
	return postFixString;
}
//--------------------------------PREFIX-----------------------------------------------------------
string Expression::prefix(void) {
	postOperation = false;
	string preFixString;
	string tempPre;
	stack <string> preStack;
	stringstream iss(expression);
	//store the normal expression and storePostFix data members
	string storeExpression = expression;
	getline(storePostFix, tempPre);
	stringstream holdStorePostFix(tempPre);

	//fill the preStack to reverse the order of the expression
	while (iss >> tempPre) {
		preStack.push(tempPre);
	}
	//empty preStack into preFixString
	while (!preStack.empty()) {
		string token = preStack.top();
		if (isParen(token)) {
			token = flipOperator(token);
		}
		preFixString += token + " ";
		preStack.pop();
	}
	//reset preFixString, expression, storePostFix for preFix needs
	expression = preFixString;
	preFixString = "";
	this->clear();
	stringstream preFixStream(this->postfix());
	//refill the now empty preStack with preFixStream
	while (preFixStream >> tempPre) {
		preStack.push(tempPre);
	}
	//empty new preStack into a new preFixString
	while (!preStack.empty()) {
		string token = preStack.top();
		preFixString += token + " ";
		preStack.pop();
	}

	//reset all the data members that got messed up by trying to do this
	this->clear();
	getline(holdStorePostFix, tempPre);
	storePostFix << tempPre;
	postOperation = true;
	expression = storeExpression;

	return preFixString;
}
//================================HELPER=FUNCTIONS=================================================
bool Expression::validPrecedence(string token, string stackToken) const {
	int tokenIndex = -1;
	int stackIndex = -1;
	//find index of operators sent to the function
	for (unsigned int i = 0; i < operators.length(); i++) {
		if (token.at(0) == operators.at(i)) {
			tokenIndex = i;
		}
		if (stackToken.at(0) == operators.at(i)) {
			stackIndex = i;
		}
	}
	if (postOperation) {
		//return relative precedence
		if (tokenIndex / 4 <= stackIndex / 4) {
			return true;
		}
		return false;
	}
	else { //if we are working on the preFix
		if (isParen(stackToken)) {
			return false;
		}
		if (tokenIndex / 4 < stackIndex / 4) {
			return true;
		}
	}
	return false;
}