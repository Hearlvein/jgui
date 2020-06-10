#include <SFML/Graphics.hpp>
#include <iostream>
#include "Button.h"


sf::RenderWindow window({600, 600}, "JGUI");
sf::Font font;
sf::RenderWindow* Widget::s_window = &window;


int main()
{
    font.loadFromFile("lucida.ttf");

	Widget popup({10.f, 10.f}, {200.f, 200.f}, sf::Color(200, 200, 200), false);
	popup.on(Event::Hover, [&]() { popup.setColor(sf::Color::Green); });
	popup.on(Event::HoverOut, [&]() { popup.setColor(sf::Color(200, 200, 200)); });

	Button button("Click me!", font, 20, {200.f, 50.f}, {300.f, 50.f}, sf::Color::Red, true);
	button.on(Event::Hover, [&]() { button.setColor(sf::Color::Yellow); });
	button.on(Event::HoverOut, [&]() { button.setColor(sf::Color::Red); });
	button.on(Event::Click, [&]() { popup.setIsVisible(!popup.getIsVisible()); });

	Button button2("Click me 2!", font, 20, {250.f, 400.f}, {300.f, 50.f}, sf::Color::Blue, true, &popup);
	button2.on(Event::Hover, [&]() { button2.setColor(sf::Color::Green); });
	button2.on(Event::HoverOut, [&]() { button2.setColor(sf::Color::Blue); });

	Button button3("Click me 3!", font, 20, {0.f, 50.f}, {150.f, 50.f}, sf::Color::Magenta, true, &button);
	button3.on(Event::Hover, [&]() { button3.setColor(sf::Color::White); });
	button3.on(Event::HoverOut, [&]() { button3.setColor(sf::Color::Magenta); });

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		button.update();
		popup.update();

		window.clear();
		window.draw(button);
		window.draw(popup);
		window.display();
	}

	return 0;
}
