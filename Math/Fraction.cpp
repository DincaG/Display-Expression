#include "Fraction.h"

Fraction::Fraction(const Expression& _up, const Expression& _down)
{
	_up.CopyInto(&up);
	_down.CopyInto(&down);

	up->setParent(*this);
	down->setParent(*this);

	float height{ down->getSize().y + up->getSize().y };
	float width{ down->getSize().x > up->getSize().x ? down->getSize().x : up->getSize().x };
	width *= 1.25f;
	setSize({ width, height });

	up->setPosition(width / 2.f, up->getSize().y / 2.f);
	down->setPosition(width / 2.f, up->getSize().y + down->getSize().y / 2.f);

	bar.setSize({ width, 5.f });
	bar.setCornerRadius(2.5f);
	bar.setParent(*this);
	bar.setFillColor(sf::Color::Black);
	bar.setPosition(width / 2.f, up->getSize().y + bar.getSize().y / 2.f);
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
	target.draw(*up, states);
	target.draw(bar, states);
	target.draw(*down, states);
}