#include "SquareRoot.h"

SquareRoot::SquareRoot(const Expression& _exp, const Text& _order)
{
	_exp.CopyInto(&exp);
	exp->setParent(*this);

	l1 = new DynamicObject;
	l1->setParent(*this);
	l1->setFillColor(sf::Color::Black);
	l2 = new DynamicObject{ *l1 };
	l3 = new DynamicObject{ *l1 };
	l4 = new DynamicObject{ *l1 };
	order = new Text{ _order };
	order->setTextSize(100.f);
	order->setParent(*this);

	setFillColor(sf::Color::Transparent);
	textSize = exp->GetTextSize();

	CalculateLayout();
}

SquareRoot::SquareRoot() 
	: l1{ nullptr }, l2{ nullptr }, l3{ nullptr }, l4{ nullptr }
	, order{ nullptr }, exp{ nullptr }
{
}

SquareRoot::~SquareRoot()
{
	delete order;
	delete exp;
	delete l1;
	delete l2;
	delete l3;
	delete l4;
}

float SquareRoot::Center() const
{
	return getSize().y / 2.f;
}

void SquareRoot::CopyInto(Expression** expression) const
{
	*expression = new SquareRoot;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	((SquareRoot*)(*expression))->l1 = new DynamicObject{ *l1 };
	((SquareRoot*)(*expression))->l1->setParent(**expression);
	((SquareRoot*)(*expression))->l2 = new DynamicObject{ *l2 };
	((SquareRoot*)(*expression))->l2->setParent(**expression);
	((SquareRoot*)(*expression))->l3 = new DynamicObject{ *l3 };
	((SquareRoot*)(*expression))->l3->setParent(**expression);
	((SquareRoot*)(*expression))->l4 = new DynamicObject{ *l4 };
	((SquareRoot*)(*expression))->l4->setParent(**expression);

	((SquareRoot*)(*expression))->order = new Text{ *order };
	((SquareRoot*)(*expression))->order->setParent(**expression);

	exp->CopyInto(&((SquareRoot*)(*expression))->exp);
	((SquareRoot*)(*expression))->exp->setParent(**expression);

	((SquareRoot*)(*expression))->textSize = textSize;
}

void SquareRoot::SetTextSize(float size)
{
	exp->SetTextSize(size);
	textSize = size;
	CalculateLayout();
}

void SquareRoot::CalculateLayout()
{
	float padding{ textSize / 3.f };
	float lineWidth{ textSize / 10.f };
	float toRadians{ 3.14159f / 180.f };
	sf::Vector2f expSize{ exp->getGlobalBounds().width, exp->getGlobalBounds().height };
	float height{ expSize.y + padding + lineWidth };

	l1->setCornerRadius(textSize / 20.f);
	l1->setSize({ textSize / 3.f, lineWidth });
	l1->setPosition(l1->getSize().x / 2.f, expSize.y / 2.f + padding + lineWidth);

	l2->setCornerRadius(textSize / 20.f);
	l2->setSize({ textSize / 10.f, (expSize.y / 2.f) / cos(-20.f * toRadians) + lineWidth / 2.f});
	l2->setRotation(-20.f);
	l2->setPosition(l1->getSize().x + l2->getGlobalBounds().width / 2.f - lineWidth, l1->getPosition().y - lineWidth / 2.f + l2->getGlobalBounds().height / 2.f);

	l3->setCornerRadius(textSize / 20.f);
	l3->setSize({ lineWidth, height / cos(10.f * toRadians) });
	l3->setRotation(10.f);
	l3->setPosition(l1->getSize().x + l2->getGlobalBounds().width + l3->getGlobalBounds().width / 2.f - lineWidth * 2.f, l3->getGlobalBounds().height / 2.f);

	l4->setCornerRadius(textSize / 20.f);
	l4->setSize({ expSize.x + lineWidth * 3.f, lineWidth });
	l4->setPosition(l1->getSize().x + l2->getGlobalBounds().width + l3->getGlobalBounds().width + l4->getSize().x / 2.f - lineWidth * 3.f, lineWidth / 2.f);

	exp->setPosition(l1->getSize().x + l2->getGlobalBounds().width + l3->getGlobalBounds().width + expSize.x / 2.f, lineWidth + padding + expSize.y / 2.f);

	order->setSize({ order->getSize().x, textSize * 0.7f });
	order->setPosition(l1->getPosition().x, l1->getPosition().y - order->getGlobalBounds().height / 2.f - lineWidth);

	float width{ l1->getSize().x + l2->getGlobalBounds().width + l3->getGlobalBounds().width + expSize.x };
	setSize({ width, height });
}

void SquareRoot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*order, states);
	target.draw(*exp, states);
	target.draw(*l1, states);
	target.draw(*l2, states);
	target.draw(*l3, states);
	target.draw(*l4, states);
}

void SquareRoot::RunAnimation()
{
	exp->RunAnimation();
}