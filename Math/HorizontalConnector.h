#pragma once
#include "Expression.h"

class HorizontalConnector : public Expression
{
public:

	HorizontalConnector(const Expression& left, const Expression& right, const Text& connector);
	HorizontalConnector();
	virtual ~HorizontalConnector();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	virtual void RunAnimation();

private:

	void CalculateLayout();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Expression* left;
	Expression* right;
	Text* connector;
};