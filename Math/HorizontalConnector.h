#pragma once
#include "Expression.h"

class HorizontalConnector : public Expression
{
public:

	HorizontalConnector(const Expression& left, const Expression& right, const Text& connector);
	HorizontalConnector();
	~HorizontalConnector();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:

	virtual void CopyInto(Expression** expression) const;

private:

	Expression* left;
	Expression* right;
	Text* connector;
};