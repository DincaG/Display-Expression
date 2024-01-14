#include "Integral.h"

Integral::Integral(const Text& _lowerBound, const Text& _upperBound, const Expression& _exp)
{
	_exp.CopyInto(&exp);
	exp->setParent(*this);

	lowerBound = new Expression{ _lowerBound };
	lowerBound->setParent(*this);

	upperBound = new Expression{ _upperBound };
	upperBound->setParent(*this);

	integral = new Text{ _lowerBound };
	integral->setParent(*this);
	integral->setTextSize(100.f);
	integral->setTextString(L"\u222B");

	dx = new Text{ *integral };
	dx->setTextString("dx");

	p1 = new Text{ *integral };
	p1->setTextString("(");

	p2 = new Text{ *integral };
	p2->setTextString(")");

	setFillColor(sf::Color::Transparent);
	textSize = exp->GetTextSize();

	CalculateLayout();
}

Integral::Integral()
	: lowerBound{ nullptr }, upperBound{ nullptr }, exp{ nullptr }
	, integral{ nullptr }, dx{ nullptr }, p1{ nullptr }, p2{ nullptr }
{
}

Integral::~Integral()
{
	delete p1;
	delete p2;
	delete integral;
	delete lowerBound;
	delete upperBound;
	delete dx;
	delete exp;
}

float Integral::Center() const
{
	return getSize().y / 2.f;
}

void Integral::CopyInto(Expression** expression) const
{
	*expression = new Integral;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	lowerBound->CopyInto(&((Integral*)(*expression))->lowerBound);
	((Integral*)(*expression))->lowerBound->setParent(**expression);
	upperBound->CopyInto(&((Integral*)(*expression))->upperBound);
	((Integral*)(*expression))->upperBound->setParent(**expression);

	((Integral*)(*expression))->integral = new Text{ *integral };
	((Integral*)(*expression))->integral->setParent(**expression);
	((Integral*)(*expression))->dx = new Text{ *dx };
	((Integral*)(*expression))->dx->setParent(**expression);

	((Integral*)(*expression))->p1 = new Text{ *p1 };
	((Integral*)(*expression))->p1->setParent(**expression);
	((Integral*)(*expression))->p2 = new Text{ *p2 };
	((Integral*)(*expression))->p2->setParent(**expression);

	exp->CopyInto(&((Integral*)(*expression))->exp);
	((Integral*)(*expression))->exp->setParent(**expression);

	((Integral*)(*expression))->textSize = textSize;
}

void Integral::SetTextSize(float size)
{
	exp->SetTextSize(size);
	lowerBound->SetTextSize(size);
	upperBound->SetTextSize(size);
	dx->setSize({ 0.f, size });
	p1->setSize({ 0.f, size });
	p2->setSize({ 0.f, size });
	integral->setSize({ 0.f, size });

	textSize = size;
	CalculateLayout();
}

void Integral::CalculateLayout()
{
	float padding{ textSize / 3.f };
	sf::Vector2f expSize{ exp->getGlobalBounds().width, exp->getGlobalBounds().height };

	integral->setSize({ integral->getTextLocalBounds().width, integral->getTextLocalBounds().height });
	float scaleY{ expSize.y / integral->getSize().y };
	if (scaleY > 3.f)
	{
		integral->setSize({ integral->getSize().x * 1.5f, integral->getSize().y * 1.5f });
		scaleY = expSize.y / integral->getSize().y;
	}
	integral->setScale(1.f, scaleY);

	float maxWidth{ std::max(lowerBound->getSize().x, upperBound->getSize().x) };
	float leftSideSize{ maxWidth / 2.f + integral->getGlobalBounds().width / 2.f };
	if (integral->getGlobalBounds().width > maxWidth)
	{
		maxWidth = leftSideSize = integral->getGlobalBounds().width;
	}

	dx->setSize({ dx->getTextGlobalBounds().width, dx->getSize().y });
	p1->setSize({ p1->getTextLocalBounds().width, p1->getTextLocalBounds().height });
	p1->setScale(1.f, expSize.y / p1->getSize().y);
	p2->setSize({ p2->getTextLocalBounds().width, p2->getTextLocalBounds().height });
	p2->setScale(1.f, expSize.y / p2->getSize().y);

	float height{ expSize.y + lowerBound->getSize().y + upperBound->getSize().y + padding * 2 };
	float width{ leftSideSize + padding * 5.f + p1->getGlobalBounds().width * 2 + expSize.x + dx->getGlobalBounds().width };
	setSize({ width, height });

	integral->setPosition(maxWidth / 2.f, height / 2.f);
	upperBound->setPosition(maxWidth / 2.f, upperBound->getSize().y / 2.f);
	lowerBound->setPosition(maxWidth / 2.f, height - upperBound->getSize().y / 2.f);
	p1->setPosition(maxWidth / 2.f + integral->getGlobalBounds().width / 2.f + padding + p1->getGlobalBounds().width / 2.f, height / 2.f);
	exp->setPosition(width - p2->getGlobalBounds().width - dx->getGlobalBounds().width - padding * 3.f - expSize.x / 2.f, height / 2.f);
	dx->setPosition(width - p2->getGlobalBounds().width - padding - dx->getGlobalBounds().width / 2.f, height / 2.f);
	p2->setPosition(width - p2->getGlobalBounds().width / 2.f, height / 2.f);
}

void Integral::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*integral, states);
	target.draw(*lowerBound, states);
	target.draw(*upperBound, states);
	target.draw(*p1, states);
	target.draw(*exp, states);
	target.draw(*dx, states);
	target.draw(*p2, states);
}

void Integral::run()
{
	exp->run();
	upperBound->run();
	lowerBound->run();
}