#pragma once
#include "Expression.h"

class Exponent : public Expression
{
public:

	Exponent(const Expression& base, const Expression& power);
	Exponent();
	~Exponent();
	virtual void CopyInto(Expression** expression) const;
	virtual float Center() const;

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	Expression* power;
	Expression* base;
};