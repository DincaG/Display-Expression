#include "HorizontalConnector.h"

HorizontalConnector::HorizontalConnector(const Expression& _left, const Expression& _right, const Text& _connector)
{
	_left.CopyInto(&left);
	_right.CopyInto(&right);

	left->setParent(*this);
	right->setParent(*this);

	connector = new Text{ _connector };
	connector->setParent(*this);
	connector->setRelativePosition(50.f, 50.f);
	connector->setSize({ connector->getTextLocalBounds().width + connector->getSize().y / 2.f, connector->getSize().y });

	float max{ std::max(left->Center(), right->Center()) };

	float height{ max + std::max(left->getSize().y - left->Center(), right->getSize().y - right->Center())};
	float width{ left->getSize().x + right->getSize().x + connector->getSize().x };
	setSize({ width, height });

	float offsetLeft{ left->getSize().y / 2.f - left->Center() };
	float offsetRight{ right->getSize().y / 2.f - right->Center() };
	left->setPosition(left->getSize().x / 2.f, max + offsetLeft);
	right->setPosition(width - right->getSize().x / 2.f, max + offsetRight);

	connector->setPosition(left->getSize().x + connector->getSize().x / 2.f, max);
}

HorizontalConnector::HorizontalConnector() : left{ nullptr }, right{ nullptr }, connector{ nullptr }
{
}

HorizontalConnector::~HorizontalConnector()
{
	delete left;
	delete right;
	delete connector;
}

void HorizontalConnector::CopyInto(Expression** expression) const
{
	*expression = new HorizontalConnector;
	*((DynamicObject*)(*expression)) = *((DynamicObject*)this);

	left->CopyInto(&((HorizontalConnector*)(*expression))->left);
	right->CopyInto(&((HorizontalConnector*)(*expression))->right);

	((HorizontalConnector*)(*expression))->left->setParent(**expression);
	((HorizontalConnector*)(*expression))->right->setParent(**expression);

	((HorizontalConnector*)(*expression))->connector = new Text{ *connector };
	((HorizontalConnector*)(*expression))->connector->setParent(**expression);
}

void HorizontalConnector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*left, states);
	target.draw(*connector, states);
	target.draw(*right, states);
}