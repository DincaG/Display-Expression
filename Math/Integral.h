#pragma once
#include "Expression.h"

class Integral : public Expression
{
public:

	Integral(const Text& lowerBound, const Text& upperBound, const Expression& exp);
	Integral();
	virtual ~Integral();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	virtual void RunAnimation();

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Text* p1;
	Text* p2;
	Text* integral;
	Text* dx;
	Expression* lowerBound;
	Expression* upperBound;
	Expression* exp;
};