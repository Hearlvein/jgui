#pragma once

#include "Widget.h"
#include <SFML/Graphics.hpp>

class Button : public Widget
{
public:
	Button(	const sf::String& title,
			const sf::Font& font,
			unsigned int characterSize = 30,
			const sf::Vector2f& position = sf::Vector2f(0, 0),
			const sf::Vector2f& size = sf::Vector2f(0, 0),
			const sf::Color& color = sf::Color(200, 200, 200),
            bool isVisible = true,
            Widget* parent = nullptr)
		: Widget(position, size, color, isVisible, parent)
		, m_text(title, font, characterSize)
	{
		onTitleUpdated();
	}
	virtual ~Button() {}

	void setTitle(const sf::String& newTitle)
	{
		m_text.setString(newTitle);
		onTitleUpdated();
	}

private:
	sf::Text m_text;

	void onTitleUpdated()
	{
		float x = (getSize().x - m_text.getLocalBounds().width) / 2;
		float y = (getSize().y - m_text.getLocalBounds().height) / 2;
		m_text.setPosition(x, y);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		Widget::draw(target, states);

		if (getIsVisible())
		{
			states.transform *= getTransform();
			target.draw(m_text, states);
		}
	}
};
