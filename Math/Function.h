#pragma once
#include "Expression.h"

class Function : public Expression
{
public:

	Function(const Text& type, const Expression& exp);
	Function();
	virtual ~Function();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	virtual void run();

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Text* p1;
	Text* p2;
	Text* type;
	Expression* exp;
};