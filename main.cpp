#include <SFML/Graphics.hpp>
#include "Framework/Button.h"
#include "Framework/Utility.h"
#include "Parser/Parser.h"
#include "Math/Math.h"

Expression* BuildExpression(Node* node, Text& textSettings)
{
	if (!node) throw "Null Node";

	switch (node->token.type)
	{
		case Token::Type::Function:
		{
			Expression* exp{ BuildExpression(node->left, textSettings) };

			if (node->token.string == "sqrt")
			{
				SquareRoot* sqrt{ new SquareRoot(*exp) };
				delete exp;
				return sqrt;
			}

			if (node->token.string.size() >= 3)
			{
				if (node->token.string.substr(0, 3) == "int")
				{
					textSettings.setTextString("");
					Text lowerBound{ textSettings };
					Text upperBound{ textSettings };

					if (node->token.string.size() > 3)
					{
						std::string interval{ node->token.string.substr(3) };
						lowerBound.setTextString(interval.substr(0, interval.find("to")));
						upperBound.setTextString(interval.substr(interval.find("to") + 2));
					}

					Integral* integral{ new Integral(lowerBound, upperBound, *exp) };
					delete exp;
					return integral;
				}
			}

			textSettings.setTextString(node->token.string);
			Function* function{ new Function(textSettings, *exp) };
			delete exp;
			return function;
		}
		case Token::Type::Operator:
		{
			Expression* left{ BuildExpression(node->left, textSettings) };
			Expression* right{ BuildExpression(node->right, textSettings) };

			if (node->token.string == "^")
			{
				if (node->left->token.type == Token::Type::Operator)
				{
					textSettings.setTextString("");
					Expression* temp{ left };
					left = new Function(textSettings, *left);
					delete temp;
				}

				Exponent* exponent{ new Exponent(*left, *right) };
				delete left;
				delete right;

				return exponent;
			}

			if (node->token.string == "/")
			{
				textSettings.setTextString(node->token.string);
				Fraction* fraction{ new Fraction(*left, *right) };
				delete left;
				delete right;

				return fraction;
			}

			if (node->token.string == "*")
			{
				textSettings.setTextString("");
				if (node->left->token.string == "-" || node->left->token.string == "+")
				{
					Expression* temp{ left };
					left = new Function(textSettings, *left);
					delete temp;
				}
				if (node->right->token.string == "-" || node->right->token.string == "+")
				{
					Expression* temp{ right };
					right = new Function(textSettings, *right);
					delete temp;
				}
			}

			if (node->token.string == "-")
			{
				if (node->right->token.string == "-" || node->right->token.string == "+")
				{
					textSettings.setTextString("");
					Expression* temp{ right };
					right = new Function(textSettings, *right);
					delete temp;
				}
			}

			textSettings.setTextString(node->token.string);
			HorizontalConnector* con{ new HorizontalConnector(*left, *right, textSettings) };
			delete left;
			delete right;

			return con;
		}
		case Token::Type::Constant:
		case Token::Type::Variable:
		{
			textSettings.setTextString(node->token.string);
			Expression* exp{ new Expression(textSettings) };

			return exp;
		}
		default:
		{
			throw "Undefined Token";
		}
	}
}

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1900, 1000), "Vizualizator de Formule", sf::Style::Default, settings);
	window.setPosition({ 0,0 });
	window.setFramerateLimit(60);

	std::string expression{ "int0to5(sqrt(-sin((( 20 + 3) * x) / 7 ^ x * (log(y) - 5/x)) / (sqrt(y*(x/2))+1) * 3))" };
	//std::string expression{ "int0to5(7^(sqrt(3)/5^2))" };
	Node* root{ nullptr };

	try
	{
		root = Parser::BuildAST(expression);
	}
	catch (const std::string& error)
	{
		std::cout << error;
		return 0;
	}
	catch (const char* error)
	{
		std::cout << error;
		return 0;
	}

	float size{ 40.f };
	Text textSettings({ 0.f, size }); //only the height matters
	textSettings.setTextFont("Resources/Lora.ttf");
	textSettings.setTextFillColor(sf::Color::Black);
	textSettings.setFillColor(sf::Color::Transparent);

	Expression* exp{ BuildExpression(root, textSettings) };
	exp->setParent(window);
	exp->setRelativePosition(50.f, 60.f);

	Text introduction{ textSettings };
	introduction.setSize({ 0.f, 80.f });
	introduction.setTextString("Reprezentarea Grafica a Expresiei");
	introduction.setParent(window);
	introduction.setRelativePosition(50.f, 15.f);

	Text stringExpression{ textSettings };
	stringExpression.setSize({ 0.f, 80.f });
	stringExpression.setTextString(expression);
	stringExpression.setParent(window);
	stringExpression.setRelativePosition(50.f, 20.f);

	Button container({ exp->getGlobalBounds().width + 150.f, exp->getGlobalBounds().height + 200.f });
	container.setFillColor(sf::Color::Transparent);
	container.setOutlineColor(sf::Color::Black);
	container.setOutlineThickness(5.f);
	container.setCornerRadius(0.f);
	container.setPointCount(20);
	container.setParent(window);
	container.setRelativePosition(50.f, 60.f);
	container.taskManager.setTaskContextData(&container);

	auto in
	{
		[](float percentageComplete, void* taskContextData)
		{
			Button* container{ (Button*)taskContextData };

			static DynamicObject initialValues;
			if (percentageComplete == 0.f)
			{
				initialValues = *(DynamicObject*)container;
			}
			container->setCornerRadius(CubicInterpolation(initialValues.getCornerRadius(), -200.f, -200.f, 170.f, percentageComplete));
			container->setRotation(CubicInterpolation(initialValues.getRotation(), -10.f, 25.f, -10.f, percentageComplete));

			float scale{ CubicInterpolation(initialValues.getScale().x, 0.8f, 1.2f, 1.f, percentageComplete) };
			container->setScale(scale, scale);
		}
	};
	auto out
	{
		[](float percentageComplete, void* taskContextData)
		{
			Button* container{ (Button*)taskContextData };

			static DynamicObject initialValues;
			if (percentageComplete == 0.f)
			{
				initialValues = *(DynamicObject*)container;
			}
			container->setCornerRadius(CubicInterpolation(initialValues.getCornerRadius(), -100.f, -100.f, 0.f, percentageComplete));
			container->setRotation(CubicInterpolation(initialValues.getRotation(), 10.f, -25.f, 10.f, percentageComplete));

			float scale{ CubicInterpolation(initialValues.getScale().x, 1.2f, 0.8f, 1.f, percentageComplete) };
			container->setScale(scale, scale);
		}
	};

	bool runAnimations{ false };
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::View view(sf::FloatRect(0, 0, (float)window.getSize().x, (float)window.getSize().y));
				window.setView(view);
			}
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				size += event.mouseWheelScroll.delta * 2;
				if (size < 0) size = 0;
				exp->SetTextSize(size);

				float r{ container.getCornerRadius() };
				container.setCornerRadius(0.f);
				container.setSize({ exp->getGlobalBounds().width + 150.f, exp->getGlobalBounds().height + 200.f });
				container.setCornerRadius(r);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				runAnimations = !runAnimations;
			}
		}
		
		Scheduler::executeTasks();
		
		if (runAnimations)
		{
			exp->RunAnimation();
			if (container.taskManager.isEmpty())
			{
				if (container.getCornerRadius() == 170.f) container.taskManager.addTask(Time::Seconds(2.f), out);
				else if (container.getCornerRadius() == 0.f) container.taskManager.addTask(Time::Seconds(2.f), in);
			}
		}

		window.clear(sf::Color(204, 204, 255));
		window.draw(introduction);
		window.draw(stringExpression);
		window.draw(container);
		window.draw(*exp);
		window.display();
	}

	return 0;
}