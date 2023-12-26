#include "Exponent.h"

Exponent::Exponent(const Expression& _base, const Expression& _power)
{
	_base.CopyInto(&base);
	_power.CopyInto(&power);

	base->setParent(*this);
	power->setParent(*this);
	power->setScale(0.6f, 0.6f);

	sf::Vector2f baseSize{ base->getGlobalBounds().width, base->getGlobalBounds().height };
	sf::Vector2f powerSize{ power->getGlobalBounds().width, power->getGlobalBounds().height };

	float height{ baseSize.y + powerSize.y };
	float width{ baseSize.x + powerSize.x };
	setSize({ width, height });
	setFillColor(sf::Color::Transparent);

	power->setPosition(baseSize.x + powerSize.x / 2.f, powerSize.y / 2.f);
	base->setPosition(baseSize.x / 2.f, powerSize.y + baseSize.y / 2.f);
}

Exponent::Exponent() : power{ nullptr }, base{ nullptr }
{
}

Exponent::~Exponent()
{
	delete power;
	delete base;
}

void Exponent::CopyInto(Expression** expression) const
{
	*expression = new Exponent;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	power->CopyInto(&((Exponent*)(*expression))->power);
	base->CopyInto(&((Exponent*)(*expression))->base);

	((Exponent*)(*expression))->power->setParent(**expression);
	((Exponent*)(*expression))->base->setParent(**expression);
}

float Exponent::Center() const
{
	return base->getPosition().y;
}

void Exponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*power, states);
	target.draw(*base, states);
}