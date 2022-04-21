//**** YOU MAY NOT MODIFY THIS DOCUMENT ****/
#ifndef EXPRESSION_INTERFACE_H
#define EXPRESSION_INTERFACE_H
#include <string>
using std::string;


class ExpressionManagerInterface
{
public:
	ExpressionManagerInterface(void) {}
	virtual ~ExpressionManagerInterface(void) {}

	/** Return the integer value of the infix expression */
	virtual int value(void) = 0;

	/** Return the infix items from the expression
		Throw an error if the expression (in this order)
		1) is missing ("Invalid Expression").
		2) is not balanced ("Unbalanced").
		3) has two adjacent operators ("Missing Operand").
		4) has an illegal operator ("Illegal Operator").
		5) has missing operator ("Missing Operator"). */
	virtual std::string infix(void) = 0;

	/** Return a postfix representation of the infix expression */
	virtual std::string postfix(void) = 0;

	/** (BONUS) Return a prefix representation of the infix expression */
	virtual std::string prefix(void)
	{
		return "NOT IMPLEMENTED";
	}

	/** Return the infix vector'd expression items */
	virtual std::string toString(void) const = 0;
};
#endif	// EXPRESSION_INTERFACE_H