#include "Parser.h"

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isAlphaNumeric(char c)
{
	return isAlpha(c) || isDigit(c);
}

Node* Parser::BuildAST(const std::string& expression)
{
	return BuildAST(Postfix(expression));
}

Node* Parser::BuildAST(const std::vector<Token>& postfix)
{
	std::vector<Node*> stack;

	for (int i{ 0 }; i < postfix.size(); i++)
	{
		if (postfix[i].type == Token::Type::Operator)
		{
			Node* node{ new Node };

			node->right = stack.back();
			stack.pop_back();

			node->left = stack.back();
			stack.pop_back();

			node->token = postfix[i];
			stack.push_back(node);
		}
		else if (postfix[i].type == Token::Type::Function)
		{
			Node* node{ new Node };

			node->left = stack.back();
			stack.pop_back();

			node->token = postfix[i];
			stack.push_back(node);
		}
		else
		{
			Node* node{ new Node };
			node->token = postfix[i];
			stack.push_back(node);
		}
	}

	return stack.back();
}

std::vector<Token> Parser::Postfix(const std::string& expression)
{
	std::vector<Token> tokens{ SplitIntoTokens(expression) };
	std::vector<Token> postfix;
	std::vector<Token> stack;

	for (int i{ 0 }; i < tokens.size(); i++)
	{
		if (tokens[i].string == "(" || tokens[i].type == Token::Type::Function)
		{
			stack.push_back(tokens[i]);
			continue;
		}

		if (tokens[i].string == ")")
		{
			while (!stack.empty())
			{
				if (stack.back().string != "(")
				{
					postfix.push_back(stack.back());
					stack.pop_back();
				}
				else break;
			}
			stack.pop_back();

			if (!stack.empty())
			{
				if (stack.back().type == Token::Type::Function)
				{
					postfix.push_back(stack.back());
					stack.pop_back();
				}
			}

			continue;
		}

		if (tokens[i].type == Token::Type::Operator)
		{
			while (!stack.empty())
			{
				if (stack.back().string == "(") break;

				if (operatorPriority[tokens[i].string[0]] <= operatorPriority[stack.back().string[0]])
				{
					if (tokens[i].string == "^") break;
					postfix.push_back(stack.back());
					stack.pop_back();
				}
				else break;
			}

			stack.push_back(tokens[i]);
		}
		else postfix.push_back(tokens[i]);
	}

	while (!stack.empty())
	{
		postfix.push_back(stack.back());
		stack.pop_back();
	}
	return postfix;
}

std::vector<Token> Parser::SplitIntoTokens(const std::string& expression)
{
	std::vector<Token> tokens;

	for (int i{ 0 }; i < expression.size(); i++)
	{
		if (expression[i] == ' ') continue;

		Token token;

		if (expression[i] == '(' || expression[i] == ')')
		{
			token.string.push_back(expression[i]);
			token.type = Token::Type::Parenthesis;
			tokens.push_back(token);
			continue;
		}

		for (int j{ 0 }; j < operators.size(); j++)
		{
			if (expression[i] == operators[j])
			{
				token.string.push_back(expression[i]);
				break;
			}
		}
		if (!token.string.empty())
		{
			token.type = Token::Type::Operator;
			tokens.push_back(token);
			continue;
		}

		if (expression[i] == '.') throw "Leading decimal point";

		bool decimal{ false };
		while (isDigit(expression[i]) || expression[i] == '.')
		{
			if (expression[i] == '.')
			{
				if (!decimal) decimal = true;
				else throw "Multiple decimal points found for the same constant";
			}
			token.string.push_back(expression[i]);
			i++;
			if (i == expression.size()) break;
		}
		if (!token.string.empty())
		{
			i--;
			token.type = Token::Type::Constant;
			tokens.push_back(token);
			continue;
		}

		while (isAlphaNumeric(expression[i]))
		{
			token.string.push_back(expression[i]);
			i++;
		}
		if (!token.string.empty())
		{
			i--;
			tokens.push_back(token);
			continue;
		}
		else throw std::string("Invalid character ") + expression[i] + " is not recognized";
	}

	ValidateTokens(tokens);

	return tokens;
}

void Parser::ValidateTokens(std::vector<Token>& tokens)
{
	int openParentheses{ 0 };

	for (int i{ 0 }; i < tokens.size(); i++)
	{
		switch (tokens[i].type)
		{
		case Token::Type::Constant:
		{
			if (i + 1 < tokens.size())
			{
				if (tokens[i + 1].string == "(") throw "Missing operator between \"" + tokens[i].string + "\" and open parenthesis";
				if (tokens[i + 1].type == Token::Type::Undefined)
				{
					for (int j{ 0 }; j < functions.size(); j++)
					{
						if (tokens[i + 1].string == functions[j])
						{
							if (i + 2 < tokens.size())
							{
								if (tokens[i + 2].string != "(")
								{
									throw "Missing open parenthesis after function \"" + tokens[i + 1].string + "\"";
								}
							}
							else throw "Missing open parenthesis after function \"" + tokens[i + 1].string + "\"";
						}
					}
					throw "Missing operator between \"" + tokens[i].string + "\" and \"" + tokens[i + 1].string
						+ "\" or if \"" + tokens[i].string + tokens[i + 1].string + "\" is variable; variables must start with a letter";
				}
			}
			break;
		}
		case Token::Type::Operator:
		{
			if (i == tokens.size() - 1) throw "Missing operand after operator \"" + tokens[i].string + "\"";
			if (tokens[i + 1].string == ")") throw "Missing operand after operator \"" + tokens[i].string + "\"";

			if (tokens[i + 1].type == Token::Type::Operator)
			{
				throw "Missing operand between operator \"" + tokens[i].string + "\" and \"" + tokens[i + 1].string + "\"";
			}

			if (i == 0)
			{
				if (tokens[i].string == "+")
				{
					tokens.erase(tokens.begin());
					i--;
				}
				else if (tokens[i].string == "-")
				{
					if (tokens[i + 1].string != "(")
					{
						tokens[i + 1].string = '-' + tokens[i + 1].string;
						tokens.erase(tokens.begin());
						i--;
					}
				}
				else throw "Missing operand before operator \"" + tokens[i].string + "\"";
			}
			break;
		}
		case Token::Type::Undefined:
		{
			for (int j{ 0 }; j < functions.size(); j++)
			{
				bool negative{ false };
				if (tokens[i].string[0] == '-')
				{
					negative = true;
					tokens[i].string.erase(tokens[i].string.begin());
				}

				bool integral{ false };
				if (tokens[i].string.size() >= 3)
				{
					if (tokens[i].string.substr(0, 3) == "int")
					{
						integral = true;
						if (tokens[i].string.size() > 3)
						{
							std::string interval{ tokens[i].string.substr(3) };
							if (interval.find("to") == std::string::npos) throw "Integral interval ill defined";

							std::string higherBound{ interval.substr(interval.find("to") + 2) };
							std::string lowerBound{ interval.substr(0, interval.find("to")) };
							if(higherBound.empty() || lowerBound.empty()) throw "Integral interval ill defined";
						}
					}
				}

				if (tokens[i].string == functions[j] || integral)
				{
					tokens[i].type = Token::Type::Function;
					if (i + 1 < tokens.size())
					{
						if (tokens[i + 1].string != "(")
						{
							throw "Missing open parenthesis after function \"" + tokens[i].string + "\"";
						}
					}
					else throw "Missing open parenthesis after function \"" + tokens[i].string + "\"";
				}

				if (negative) tokens[i].string = '-' + tokens[i].string;
			}
			if (tokens[i].type == Token::Type::Undefined)
			{
				tokens[i].type = Token::Type::Variable;
				if (i + 1 < tokens.size())
				{
					if (tokens[i + 1].type == Token::Type::Undefined)
					{
						throw "Missing operator between variable \"" + tokens[i].string + "\" and \"" + tokens[i + 1].string + "\"";
					}
					if (tokens[i + 1].string == "(")
					{
						throw "Missing operator between variable \"" + tokens[i].string + "\" and open parenthesis";
					}
				}
			}
			break;
		}
		case Token::Type::Parenthesis:
		{
			if (tokens[i].string == "(")
			{
				openParentheses++;
				if (i + 1 < tokens.size())
				{
					if (tokens[i + 1].type == Token::Type::Operator)
					{
						if (tokens[i + 1].string == "-")
						{
							if (i + 2 < tokens.size())
							{
								if (tokens[i + 2].type == Token::Type::Constant || tokens[i + 2].type == Token::Type::Undefined)
								{
									tokens[i + 2].string = '-' + tokens[i + 2].string;
									tokens.erase(tokens.begin() + i + 1);
								}
							}
						}
						else if (tokens[i + 1].string == "+")
						{
							if (i + 2 < tokens.size())
							{
								if (tokens[i + 2].type == Token::Type::Constant ||
									tokens[i + 2].type == Token::Type::Undefined ||
									tokens[i + 2].string == "(")
								{
									tokens.erase(tokens.begin() + i + 1);
								}
							}
						}
						else throw "Missing operand before operator \"" + tokens[i + 1].string + "\"";
					}
					else if (tokens[i + 1].string == ")") throw "Remove empty parentheses";
				}
				else throw "Parentheses mismatch";
			}
			else if (tokens[i].string == ")")
			{
				openParentheses--;
				if (openParentheses < 0) throw "Parentheses mismatch";

				if (i + 1 < tokens.size())
				{
					if (tokens[i + 1].type == Token::Type::Constant ||
						tokens[i + 1].type == Token::Type::Undefined ||
						tokens[i + 1].string == "(")
					{
						throw "Missing operator between \")\" and \"" + tokens[i + 1].string + "\"";
					}
				}
			}
			break;
		}

		default:
			break;
		}
	}

	if (openParentheses != 0) throw "Parentheses mismatch";
}

const std::vector<std::string> Parser::functions 
{
	"sin",
	"cos",
	"tan",
	"log",
	"ln",
	"sqrt",
	"abs"
};

const std::vector<char> Parser::operators
{
	'+',
	'-',
	'*',
	'/',
	'^'
};

std::map<char, int> Parser::operatorPriority
{
	{ '+', 0 },
	{ '-', 0 },
	{ '*', 1 },
	{ '/', 1 },
	{ '^', 2 },
};