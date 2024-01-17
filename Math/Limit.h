#pragma once
#include "Expression.h"

class Limit : public Expression
{
public:

	Limit(const Text& lowerBound, const Text& upperBound, const Expression& exp);
	Limit();
	virtual ~Limit();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	virtual void RunAnimation();

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Text* p1;
	Text* p2;
	Text* arrow;
	Text* lim;
	Expression* lowerBound;
	Expression* upperBound;
	Expression* exp;
};