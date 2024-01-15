#include "Exponent.h"

Exponent::Exponent(const Expression& _base, const Expression& _power)
{
	_base.CopyInto(&base);
	_power.CopyInto(&power);

	base->setParent(*this);
	power->setParent(*this);

	setFillColor(sf::Color::Transparent);
	textSize = base->GetTextSize();
	power->SetTextSize(textSize * 0.6f);
	
	CalculateLayout();
}

Exponent::Exponent() : power{ nullptr }, base{ nullptr }
{
}

Exponent::~Exponent()
{
	delete power;
	delete base;
}

float Exponent::Center() const
{
	return base->getPosition().y;
}

void Exponent::CopyInto(Expression** expression) const
{
	*expression = new Exponent;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	power->CopyInto(&((Exponent*)(*expression))->power);
	base->CopyInto(&((Exponent*)(*expression))->base);

	((Exponent*)(*expression))->power->setParent(**expression);
	((Exponent*)(*expression))->base->setParent(**expression);

	((Exponent*)(*expression))->textSize = textSize;
}

void Exponent::SetTextSize(float size)
{
	base->SetTextSize(size);
	power->SetTextSize(size * 0.6f);
	textSize = size;
	CalculateLayout();
}

void Exponent::CalculateLayout()
{
	sf::Vector2f baseSize{ base->getGlobalBounds().width, base->getGlobalBounds().height };
	sf::Vector2f powerSize{ power->getGlobalBounds().width, power->getGlobalBounds().height };

	float height{ baseSize.y + powerSize.y };
	float width{ baseSize.x + powerSize.x };
	setSize({ width, height });

	power->setPosition(baseSize.x + powerSize.x / 2.f, powerSize.y / 2.f);
	base->setPosition(baseSize.x / 2.f, powerSize.y + baseSize.y / 2.f);
}

void Exponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*power, states);
	target.draw(*base, states);
}

void Exponent::RunAnimation()
{
	power->RunAnimation();
	base->RunAnimation();
}