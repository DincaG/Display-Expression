#pragma once
#include "Expression.h"

class Fraction : public Expression
{
public:

	Fraction(const Expression& up, const Expression& down);
	Fraction();
	virtual ~Fraction();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	virtual void run();

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Expression* up;
	Expression* down;
	DynamicObject* bar;
};
