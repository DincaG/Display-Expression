#pragma once
#include "../Framework/Text.h"
#include "../Framework/Utility.h"
#include <random>

class Expression : public Text
{
public:

	Expression(const Text& text);
	Expression();
	virtual ~Expression();
	virtual float Center() const;
	virtual void CopyInto(Expression** expression) const;
	virtual void SetTextSize(float size);
	float GetTextSize() const;
	virtual void RunAnimation();

protected:

	float textSize;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	Text* text;
};

static std::mt19937 mt{ std::random_device{}() };
static std::uniform_int_distribution randNumber{ 0, 700 };