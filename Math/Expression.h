#pragma once
//#include "../Framework/Text.h"
#include "../Framework/Button.h"

class Expression : public Button
{
public:

	Expression(const Text& text);
	Expression();
	virtual ~Expression();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	float GetTextSize() const;
	virtual void run();

protected:

	float textSize;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Text* text;
};