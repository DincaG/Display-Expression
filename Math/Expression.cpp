#include "Expression.h"

Expression::Expression(const Text& _text)
{
    text = new Text{ _text };
    text->setParent(*this);
    text->setRelativePosition(50.f, 50.f);
    text->setTextSize(100.f);
    setSize({ text->getTextLocalBounds().width, text->getTextLocalBounds().height });
    setFillColor(sf::Color::Transparent);
}

Expression::Expression() : text{ nullptr }
{
}

Expression::~Expression()
{
    delete text;
}

void Expression::CopyInto(Expression** expression) const
{
    *expression = new Expression;
    *((DynamicObject*)(*expression)) = *((DynamicObject*)this);

    (*expression)->text = new Text{ *text };
    (*expression)->text->setParent(**expression);
}

float Expression::Center() const
{
    return getSize().y / 2.f;
}

void Expression::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
    applyChanges(combinedTransform);

    Shape::draw(target, combinedTransform);
    if (text) target.draw(*text, states);
}