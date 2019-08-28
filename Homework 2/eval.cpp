#include <iostream>
#include <string>
#include <stack>
#include <cassert>
using namespace std;

int evaluate(string infix, string& postfix, bool& result)
// Evaluates a boolean expression
//   If infix is a syntactically valid infix boolean expression,
//   then set postfix to the postfix form of that expression, set
//   result to the value of the expression, and return zero.  If
//   infix is not a syntactically valid expression, return 1; in
//   that case, postfix may or may not be changed, but result must
//   be unchanged.
{
	bool validInfix = true;
	stack<char> checkString; // check if the operators and operand are in order
	stack<char> checkParentheses; // check if the parentheses match
	for (size_t i = 0; i < infix.length() && validInfix != false; i++) // loop through the string infix
	{
		switch (infix[i])
		{
		case 'F': // F || T : if empty string or top of string stack is not an operand, push it to stack
		case 'T':
			if (checkString.empty() || (checkString.top() != 'T' && checkString.top() != 'F'))
			{
				checkString.push(infix[i]);
				break;
			}
			else
			{
				validInfix = false;
				break;
			}
		case '^': // ^ || & : if not empty and the string before it is not an operator 
		case '&':
			if (!checkString.empty() && (checkString.top() != '^' && checkString.top() != '&'))
			{
				checkString.push(infix[i]);
				break;
			}
			else
			{
				validInfix = false;
				break;
			}
		case '(': // ( : if checkString stack is empty or the top of it is ^ or & then push the parentheses onto the checkParentheses stack
			if (checkString.empty() || checkString.top() == '^' || checkString.top() == '&' || checkString.top() == '!')
			{
				checkParentheses.push(infix[i]);
				break;
			}
			else
			{
				validInfix = false;
				break;
			}
		case ')': // ) : if there are parentheses in the parentheses stack and the top of the string stack is an F or T then pop from the parentheses stack
			if (checkString.empty()) // if there is nothing between parentheses then infix is not valid
			{
				validInfix = false;
				break;
			}
			if (!checkParentheses.empty() && (checkString.top() == 'F' || checkString.top() == 'T'))
			{
				checkParentheses.pop();
				break;
			}
			else
			{
				validInfix = false;
				break;
			}
		case '!':
			if (checkString.empty() || (checkString.top() != 'T' && checkString.top() != 'F' && checkString.top() != ')'))
			{
				checkString.push(infix[i]);
				break;
			}
			else
			{
				validInfix = false;
				break;
			}
		default: // for everything else (like spaces), continue
			break;
		}
	}
	if (!checkParentheses.empty()) // if parentheses stack is not empty then infix is not valid
		validInfix = false;
	if (!checkString.empty()) // if string stack ends with ^ or & then infix is not valid
	{
		if (checkString.top() == '^' || checkString.top() == '&')
			validInfix = false;
	}
	else
		validInfix = false; // infix not valid if empty
	

	if (!validInfix)// if validInfix is false, then return 1
		return 1;

	// now infix is syntactically correct, convert to postfix and evaluate
	char temp; // temp for storing values that have been popped
	stack<char> operatorStack; // for operatorStack
	stack<char> pf; // for postfix experssion
	for (size_t i = 0; i < infix.length() && validInfix != false; i++) // loop through the string infix
	{
		switch (infix[i])
		{
		case 'T': // if T or F : push the operand onto the postfix stack
		case 'F':
			pf.push(infix[i]);
			break;
		case '^': // ^ : lowest precedence operator. Check for higher or equal precedences in operator stack, if there are, pop that operator and check again. If not, push this onto the operator stack
			if (!operatorStack.empty() && (operatorStack.top() == '!' || operatorStack.top() == '&' || operatorStack.top() == '^'))
			{
				temp = operatorStack.top();
				operatorStack.pop();
				pf.push(temp);
				i--;
				break;
			}
			else
			{
				operatorStack.push(infix[i]);
				break;
			}
		case '&': // & : higher precedence than ^. Check for higher or equal precedences in operator stack, if there are, pop that operator and check again. If not, push this onto the operator stack
			if (!operatorStack.empty() && (operatorStack.top() == '!' || operatorStack.top() == '&'))
			{
				temp = operatorStack.top();
				operatorStack.pop();
				pf.push(temp);
				i--;
				break;
			}
			else
			{
				operatorStack.push(infix[i]);
				break;
			}
		case '!':
			operatorStack.push(infix[i]);
			break;
		case '(': // ( : push onto the operator stack
			operatorStack.push(infix[i]);
			break;
		case ')': // ) : keep popping operator stack until it reaches (, then remove ( and continue
			while (operatorStack.top() != '(')
			{
				temp = operatorStack.top();
				operatorStack.pop();
				pf.push(temp);
			}
			operatorStack.pop();
			break;
		default:
			break;
		}
	}
	// convert the stack to a string to put into postfix
	while (!operatorStack.empty())
	{
		temp = operatorStack.top();
		operatorStack.pop();
		pf.push(temp);
	}
	stack<char> pop;
	while (!pf.empty())
	{
		temp = pf.top();
		pf.pop();
		pop.push(temp);
	}
	postfix = "";
	while (!pop.empty())
	{
		temp = pop.top();
		pop.pop();
		postfix += temp;	
	}
	//cerr << postfix << endl;

	// evaluate the postfix expression
	stack<char> evaluate;
	char operand1, operand2; // 1 is the one preceding the operator, and 2 is the one preceding 1
	for (size_t i = 0; i < postfix.length() && validInfix != false; i++) // loop through the string postfix
	{
		switch (postfix[i])
		{
		case 'T': // if T or F : push onto the stack
		case 'F':
			evaluate.push(postfix[i]);
			break;
		case '!': // if ! : take the top character in the stack and evaluate the ! of it
			//if (evaluate.empty())
			temp = evaluate.top();
			evaluate.pop();
			if (temp == 'T')
				temp = 'F';
			else
				temp = 'T';
			evaluate.push(temp);
			break;
		case '&': // if & : take the top two characters and evaluate the & of them
			operand1 = evaluate.top();
			evaluate.pop();
			operand2 = evaluate.top();
			evaluate.pop();
			if (operand1 == 'T' && operand1 == operand2) // and operation
				evaluate.push('T');
			else
				evaluate.push('F');
			break;
		case '^': // if ^ : take the top two characters and evaluate the ^ of them
			operand1 = evaluate.top();
			evaluate.pop();
			operand2 = evaluate.top();
			evaluate.pop();
			if (operand1 != operand2) // xor operation
				evaluate.push('T');
			else
				evaluate.push('F');
			break;
		default:
			break;
		}
	}
	temp = evaluate.top(); // get the evaluated value from the stack and set result based on this result
	if (temp == 'T')
		result = true;
	if (temp == 'F')
		result = false;
	return 0;
}

int main()
{
	string pf;
	bool answer;
	assert(evaluate("T^ F", pf, answer) == 0 && pf == "TF^"  &&  answer);
	assert(evaluate("T^", pf, answer) == 1);
	assert(evaluate("F F", pf, answer) == 1);
	assert(evaluate("TF", pf, answer) == 1);
	assert(evaluate("()", pf, answer) == 1);
	assert(evaluate("()T", pf, answer) == 1);
	assert(evaluate("T(F^T)", pf, answer) == 1);
	assert(evaluate("T(&T)", pf, answer) == 1);
	assert(evaluate("(T&(F^F)", pf, answer) == 1);
	assert(evaluate("T|F", pf, answer) == 1);
	assert(evaluate("", pf, answer) == 1);
	assert(evaluate("F  ^  !F & (T&F) ", pf, answer) == 0
		&& pf == "FF!TF&&^" && !answer);
	assert(evaluate(" F  ", pf, answer) == 0 && pf == "F" && !answer);
	assert(evaluate("((T))", pf, answer) == 0 && pf == "T"  &&  answer);
	cout << "Passed all tests" << endl;
}