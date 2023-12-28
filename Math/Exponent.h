#pragma once
#include "Expression.h"

class Exponent : public Expression
{
public:

	Exponent(const Expression& base, const Expression& power);
	Exponent();
	virtual ~Exponent();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Expression* power;
	Expression* base;
};