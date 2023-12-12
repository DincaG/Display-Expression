#pragma once
#include "Expression.h"

class Fraction : public Expression
{
public:

	Fraction(const Expression& up, const Expression& down);
	Fraction();
	~Fraction();
	virtual void CopyInto(Expression** expression) const;
	virtual float Center() const;

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	Expression* up;
	Expression* down;
	DynamicObject bar;
};
