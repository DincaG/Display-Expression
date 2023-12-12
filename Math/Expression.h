#pragma once
#include "Framework/Text.h"

class Expression : public DynamicObject
{
public:

	Expression(const Text& text);
	Expression();
	virtual ~Expression();
	virtual void CopyInto(Expression** expression) const;
	virtual float Center() const;

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

	Text* text;
};