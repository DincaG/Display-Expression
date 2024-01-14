#include "Fraction.h"

Fraction::Fraction(const Expression& _up, const Expression& _down)
{
	_up.CopyInto(&up);
	_down.CopyInto(&down);

	up->setParent(*this);
	down->setParent(*this);

	bar = new DynamicObject;
	bar->setParent(*this);
	bar->setFillColor(sf::Color::Black);

	setFillColor(sf::Color::Transparent);
	textSize = up->GetTextSize();

	CalculateLayout();
}

Fraction::Fraction() : up{ nullptr }, down{ nullptr }, bar{ new DynamicObject }
{
}

Fraction::~Fraction()
{
	delete up;
	delete down;
	delete bar;
}

float Fraction::Center() const
{
	return bar->getPosition().y;
}

void Fraction::CopyInto(Expression** expression) const
{
	*expression = new Fraction;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	up->CopyInto(&((Fraction*)(*expression))->up);
	down->CopyInto(&((Fraction*)(*expression))->down);

	((Fraction*)(*expression))->up->setParent(**expression);
	((Fraction*)(*expression))->down->setParent(**expression);

	((Fraction*)(*expression))->bar = new DynamicObject{ *bar };
	((Fraction*)(*expression))->bar->setParent(**expression);

	((Fraction*)(*expression))->textSize = textSize;
}

void Fraction::SetTextSize(float size)
{
	up->SetTextSize(size);
	down->SetTextSize(size);
	textSize = size;
	CalculateLayout();
}

void Fraction::CalculateLayout()
{
	float padding{ textSize / 4.f };
	float lineWidth{ textSize / 10.f };
	sf::Vector2f upSize{ up->getGlobalBounds().width, up->getGlobalBounds().height };
	sf::Vector2f downSize{ down->getGlobalBounds().width, down->getGlobalBounds().height };

	float height{ downSize.y + upSize.y + padding * 2.f + lineWidth };
	float width{ downSize.x > upSize.x ? downSize.x : upSize.x };
	width *= 1.25f;
	setSize({ width, height });

	bar->setCornerRadius(textSize / 20.f);
	bar->setSize({ width, lineWidth });
	bar->setPosition(width / 2.f, padding + upSize.y + bar->getSize().y / 2.f);

	up->setPosition(width / 2.f, upSize.y / 2.f);
	down->setPosition(width / 2.f, padding * 2.f + bar->getSize().y + upSize.y + downSize.y / 2.f);
}

void Fraction::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*up, states);
	target.draw(*bar, states);
	target.draw(*down, states);
}

void Fraction::run()
{
	up->run();
	down->run();
}