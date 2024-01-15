#pragma once
#include "Expression.h"

class SquareRoot : public Expression
{
public:

	SquareRoot(const Expression& exp);
	SquareRoot();
	virtual ~SquareRoot();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	virtual void RunAnimation();

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	DynamicObject* l1;
	DynamicObject* l2;
	DynamicObject* l3;
	DynamicObject* l4;
	Expression* exp;
};