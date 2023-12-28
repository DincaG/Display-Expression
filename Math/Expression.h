#pragma once
#include "../Framework/Text.h"

class Expression : public DynamicObject
{
public:

	Expression(const Text& text);
	Expression();
	virtual ~Expression();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	float GetTextSize() const;

protected:

	float textSize;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Text* text;
};