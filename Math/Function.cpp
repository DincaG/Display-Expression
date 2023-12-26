#include "Function.h"

Function::Function(const Text& _type, const Expression& _exp)
{
	_exp.CopyInto(&exp);
	exp->setParent(*this);

	type = new Text{ _type };
	type->setParent(*this);
	type->setTextSize(100.f);
	type->setSize({ type->getTextLocalBounds().width + type->getTextLocalBounds().height / 3.f, type->getGlobalBounds().height });

	p1 = new Text(_type);
	p1->setTextSize(100.f);
	p1->setParent(*this);
	p1->setTextString("(");
	p1->setSize({ p1->getTextLocalBounds().width, p1->getTextLocalBounds().height });
	p1->setScale(1.f, exp->getGlobalBounds().height / p1->getSize().y);

	p2 = new Text(*p1);
	p2->setTextString(")");

	float height{ exp->getGlobalBounds().height };
	float width{ type->getGlobalBounds().width + p1->getGlobalBounds().width * 2 + exp->getGlobalBounds().width };
	setSize({ width, height });

	type->setPosition(type->getGlobalBounds().width / 2.f, exp->getGlobalBounds().height / 2.f );
	p1->setPosition(type->getGlobalBounds().width + p1->getGlobalBounds().width / 2.f, p1->getGlobalBounds().height / 2.f);
	exp->setPosition(width - p2->getGlobalBounds().width - exp->getGlobalBounds().width / 2.f, exp->getGlobalBounds().height / 2.f);
	p2->setPosition(width - p2->getGlobalBounds().width / 2.f, p2->getGlobalBounds().height / 2.f);
}

Function::Function() : type{ nullptr }, exp{ nullptr }, p1{ nullptr }, p2{ nullptr }
{
}

Function::~Function()
{
	delete type;
	delete exp;
	delete p1;
	delete p2;
}

void Function::CopyInto(Expression** expression) const
{
	*expression = new Function;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	((Function*)(*expression))->type = new Text{ *type };
	((Function*)(*expression))->type->setParent(**expression);

	((Function*)(*expression))->p1 = new Text{ *p1 };
	((Function*)(*expression))->p1->setParent(**expression);
	((Function*)(*expression))->p2 = new Text{ *p2 };
	((Function*)(*expression))->p2->setParent(**expression);

	exp->CopyInto(&((Function*)(*expression))->exp);
	((Function*)(*expression))->exp->setParent(**expression);
}

float Function::Center() const
{
	return type->getPosition().y;
}

void Function::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*type, states);
	target.draw(*p1, states);
	target.draw(*exp, states);
	target.draw(*p2, states);
}