#include "Fraction.h"

Fraction::Fraction(const Expression& _up, const Expression& _down)
{
	_up.CopyInto(&up);
	_down.CopyInto(&down);

	up->setParent(*this);
	down->setParent(*this);

	float padding{ 0.25f * 80.f };
	sf::Vector2f upSize{ up->getGlobalBounds().width, up->getGlobalBounds().height };
	sf::Vector2f downSize{ down->getGlobalBounds().width, down->getGlobalBounds().height };

	float height{ downSize.y + upSize.y + padding * 2.f };
	float width{ downSize.x > upSize.x ? downSize.x : upSize.x };
	width *= 1.25f;
	setSize({ width, height });
	setFillColor(sf::Color::Transparent);

	bar.setSize({ width, 5.f });
	bar.setCornerRadius(2.5f);
	bar.setParent(*this);
	bar.setFillColor(sf::Color::Black);
	bar.setPosition(width / 2.f, padding + upSize.y + bar.getSize().y / 2.f);

	up->setPosition(width / 2.f, upSize.y / 2.f);
	down->setPosition(width / 2.f, padding * 2.f + bar.getSize().y / 2.f + upSize.y + downSize.y / 2.f);
}

Fraction::Fraction() : up{ nullptr }, down{ nullptr }
{
}

Fraction::~Fraction()
{
	delete up;
	delete down;
}

void Fraction::CopyInto(Expression** expression) const
{
	*expression = new Fraction;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	up->CopyInto(&((Fraction*)(*expression))->up);
	down->CopyInto(&((Fraction*)(*expression))->down);

	((Fraction*)(*expression))->up->setParent(**expression);
	((Fraction*)(*expression))->down->setParent(**expression);

	((Fraction*)(*expression))->bar = bar;
	((Fraction*)(*expression))->bar.setParent(**expression);
}

float Fraction::Center() const
{
	return bar.getPosition().y;
}

void Fraction::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*up, states);
	target.draw(bar, states);
	target.draw(*down, states);
}