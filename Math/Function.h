#pragma once
#include "Expression.h"

class Function : public Expression
{
public:

	Function(const Text& type, const Expression& exp);
	Function();
	~Function();
	virtual void CopyInto(Expression** expression) const;
	virtual float Center() const;

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	Text* p1;
	Text* p2;
	Text* type;
	Expression* exp;
};