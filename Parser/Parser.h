#pragma once
#include <string>
#include <vector>
#include <map>
#include <queue>

struct Token
{
	enum class Type
	{
		Constant,
		Variable,
		Operator,
		Function,
		Parenthesis,
		Undefined
	};
	std::string string;
	Type type{ Type::Undefined };
};

struct Node
{
	Node* left{ nullptr };
	Node* right{ nullptr };
	Token token;
};

class Parser
{
public:

	static Node* BuildAST(const std::string& expression);
	static Node* BuildAST(const std::vector<Token>& postfix);
	static std::vector<Token> Postfix(const std::string& expression);

private:

	static std::vector<Token> SplitIntoTokens(const std::string& expression);
	static void ValidateTokens(std::vector<Token>& tokens);

	const static std::vector<std::string> functions;
	const static std::vector<char> operators;
	static std::map<char, int> operatorPriority;
};