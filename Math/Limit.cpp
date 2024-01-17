#include "Limit.h"

Limit::Limit(const Text& _lowerBound, const Text& _upperBound, const Expression& _exp)
{
	_exp.CopyInto(&exp);
	exp->setParent(*this);

	lowerBound = new Expression{ _lowerBound };
	lowerBound->setParent(*this);

	upperBound = new Expression{ _upperBound };
	upperBound->setParent(*this);

	p1 = new Text{ _lowerBound };
	p1->setTextSize(100.f);
	p1->setParent(*this);
	p1->setTextString("(");

	p2 = new Text{ *p1 };
	p2->setTextString(")");

	lim = new Text{ *p1 };
	lim->setTextString("lim");

	arrow = new Text{ *p1 };
	arrow->setTextString(L"\u2192");

	setFillColor(sf::Color::Transparent);
	textSize = exp->GetTextSize();

	CalculateLayout();
}

Limit::Limit()
	: lowerBound{ nullptr }, upperBound{ nullptr }, arrow{ nullptr }
	, p1{ nullptr }, p2{ nullptr }, exp{ nullptr }, lim{ nullptr }
{
}

Limit::~Limit()
{
	delete p1;
	delete p2;
	delete lowerBound;
	delete upperBound;
	delete exp;
	delete arrow;
	delete lim;
}

float Limit::Center() const
{
	return getSize().y / 2.f;
}

void Limit::CopyInto(Expression** expression) const
{
	*expression = new Limit;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	lowerBound->CopyInto(&((Limit*)(*expression))->lowerBound);
	((Limit*)(*expression))->lowerBound->setParent(**expression);
	upperBound->CopyInto(&((Limit*)(*expression))->upperBound);
	((Limit*)(*expression))->upperBound->setParent(**expression);

	((Limit*)(*expression))->arrow = new Text{ *arrow };
	((Limit*)(*expression))->arrow->setParent(**expression);

	((Limit*)(*expression))->p1 = new Text{ *p1 };
	((Limit*)(*expression))->p1->setParent(**expression);
	((Limit*)(*expression))->p2 = new Text{ *p2 };
	((Limit*)(*expression))->p2->setParent(**expression);

	exp->CopyInto(&((Limit*)(*expression))->exp);
	((Limit*)(*expression))->exp->setParent(**expression);

	((Limit*)(*expression))->textSize = textSize;
}

void Limit::SetTextSize(float size)
{
	exp->SetTextSize(size);
	p1->setSize({ 0.f, size });
	p2->setSize({ 0.f, size });

	textSize = size;
	CalculateLayout();
}

void Limit::CalculateLayout()
{
	float padding{ textSize / 3.f };
	sf::Vector2f expSize{ exp->getGlobalBounds().width, exp->getGlobalBounds().height };
	
	lowerBound->SetTextSize(textSize * 0.8f);
	upperBound->SetTextSize(textSize * 0.8f);

	lim->setSize({ lim->getTextLocalBounds().width, textSize });
	arrow->setSize({ arrow->getTextLocalBounds().width, textSize * 0.8f });

	p1->setSize({ p1->getTextLocalBounds().width, p1->getTextLocalBounds().height });
	p1->setScale(1.f, expSize.y / p1->getSize().y);
	p2->setSize({ p2->getTextLocalBounds().width, p2->getTextLocalBounds().height });
	p2->setScale(1.f, expSize.y / p2->getSize().y);
	
	float height{ expSize.y + padding * 2.f };
	float width{ lowerBound->getGlobalBounds().width + arrow->getGlobalBounds().width + upperBound->getGlobalBounds().width + padding * 4.f + p1->getGlobalBounds().width * 2 + expSize.x};
	setSize({ width, height });
	
	lowerBound->setPosition(lowerBound->getGlobalBounds().width / 2.f, height / 2.f + padding);
	arrow->setPosition(lowerBound->getGlobalBounds().width + padding / 2.f + arrow->getGlobalBounds().width / 2.f, height / 2.f + padding);
	upperBound->setPosition(arrow->getPosition().x + arrow->getGlobalBounds().width / 2.f + padding / 2.f + upperBound->getGlobalBounds().width / 2.f, height / 2.f + padding);
	lim->setPosition(arrow->getPosition().x, height / 2.f - padding * 1.5f);

	p1->setPosition(upperBound->getPosition().x + upperBound->getGlobalBounds().width / 2.f + padding + p1->getGlobalBounds().width / 2.f, height / 2.f);
	exp->setPosition(upperBound->getPosition().x + upperBound->getGlobalBounds().width / 2.f + padding * 2.f + p1->getGlobalBounds().width + exp->getGlobalBounds().width / 2.f, height / 2.f);
	p2->setPosition(width - p2->getGlobalBounds().width / 2.f, height / 2.f);
}

void Limit::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*lim, states);
	target.draw(*arrow, states);
	target.draw(*lowerBound, states);
	target.draw(*upperBound, states);
	target.draw(*p1, states);
	target.draw(*exp, states);
	target.draw(*p2, states);
}

void Limit::RunAnimation()
{
	exp->RunAnimation();
	upperBound->RunAnimation();
	lowerBound->RunAnimation();

	auto flip2
	{
		[](float percentageComplete, void* taskContextData)
		{
			DynamicObject* object{ (DynamicObject*)taskContextData };

			static DynamicObject initialValues;
			if (percentageComplete == 0.f)
			{
				initialValues = *object;
			}

			object->setRotation(CubicInterpolation(initialValues.getRotation(), -50.f, 50.f, 0.f, percentageComplete));
		}
	};
	p1->taskManager.setTaskContextData(p1);
	if (p1->taskManager.isEmpty() && randNumber(mt) % 700 == 0) p1->taskManager.addTask(Time::Seconds(1.5f), flip2);
	p2->taskManager.setTaskContextData(p2);
	if (p2->taskManager.isEmpty() && randNumber(mt) % 700 == 0) p2->taskManager.addTask(Time::Seconds(1.5f), flip2);
}