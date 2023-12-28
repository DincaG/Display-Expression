#include "Expression.h"

Expression::Expression(const Text& _text)
{
    text = new Text{ _text };
    text->setParent(*this);
    text->setRelativePosition(50.f, 50.f);
    text->setTextSize(100.f);

    setFillColor(sf::Color::Transparent);
    setSize({ text->getTextLocalBounds().width, text->getTextLocalBounds().height });
    textSize = _text.getSize().y;
}

Expression::Expression() : text{ nullptr }, textSize{ 0.f }
{
}

Expression::~Expression()
{
    delete text;
}

float Expression::Center() const
{
    return getSize().y / 2.f;
}

void Expression::CopyInto(Expression** expression) const
{
    *expression = new Expression;
    *((DynamicObject*)(*expression)) = *((DynamicObject*)this);

    (*expression)->text = new Text{ *text };
    (*expression)->text->setParent(**expression);

    (*expression)->textSize = textSize;
}

void Expression::SetTextSize(float size)
{
    text->setSize({ 0.f,size });
    setSize({ text->getTextLocalBounds().width, text->getTextLocalBounds().height });
    textSize = size;
}

float Expression::GetTextSize() const
{
    return textSize;
}

void Expression::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
    applyChanges(combinedTransform);

    Shape::draw(target, combinedTransform);
    if (text) target.draw(*text, states);
}