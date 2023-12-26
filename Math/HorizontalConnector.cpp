#include "HorizontalConnector.h"

HorizontalConnector::HorizontalConnector(const Expression& _left, const Expression& _right, const Text& _connector)
{
	_left.CopyInto(&left);
	_right.CopyInto(&right);

	left->setParent(*this);
	right->setParent(*this);

	sf::Vector2f leftSize{ left->getGlobalBounds().width, left->getGlobalBounds().height };
	sf::Vector2f rightSize{ right->getGlobalBounds().width, right->getGlobalBounds().height };

	connector = new Text{ _connector };
	connector->setParent(*this);
	connector->setTextSize(100.f);
	connector->setSize({ connector->getTextLocalBounds().width + connector->getGlobalBounds().height / 2.f, connector->getGlobalBounds().height });

	float max{ std::max(left->Center(), right->Center()) };

	float height{ max + std::max(leftSize.y - left->Center(), rightSize.y - right->Center())};
	float width{ leftSize.x + rightSize.x + connector->getGlobalBounds().width };
	setSize({ width, height });
	setFillColor(sf::Color::Transparent);

	float offsetLeft{ leftSize.y / 2.f - left->Center() };
	float offsetRight{ rightSize.y / 2.f - right->Center() };
	left->setPosition(leftSize.x / 2.f, max + offsetLeft);
	right->setPosition(width - rightSize.x / 2.f, max + offsetRight);

	connector->setPosition(leftSize.x + connector->getGlobalBounds().width / 2.f, max);
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
	sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
	applyChanges(combinedTransform);

	Shape::draw(target, combinedTransform);
	target.draw(*left, states);
	target.draw(*connector, states);
	target.draw(*right, states);
}