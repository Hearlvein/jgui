#pragma once

#include "debug.h"
#include <SFML/Graphics.hpp>
#include <iostream>

enum Event
{
	Hover,
	HoverOut,
	Click
};

class Widget : public sf::Drawable, public sf::Transformable
{
public:
    static sf::RenderWindow* s_window;

	Widget(	const sf::Vector2f& position = sf::Vector2f(0, 0),
			const sf::Vector2f& size = sf::Vector2f(0, 0),
			const sf::Color& color = sf::Color(200, 200, 200),
			bool isVisible = true,
			Widget* parent = nullptr)
		: m_size(size)
	{
		m_vertices[0] = { { 0.f, 0.f },				color };
		m_vertices[1] = { { m_size.x, 0.f },		color };
		m_vertices[2] = { { m_size.x, m_size.y },	color };
		m_vertices[3] = { { 0.f, m_size.y },		color };

		setParent(parent);	// called before setIsVisible, so children inherit from later change
		setIsVisible(isVisible);

		if (getParent())
			setPosition(getParent()->getPosition() + position);
		else
			setPosition(position);
	}
	virtual ~Widget()
	{
	    std::cerr << "destructing a widget" << std::endl;
	}

	void update()
	{
	    if (!s_window || !getIsVisible())
            return;

		sf::Vector2f mouse = {
			static_cast<float>(sf::Mouse::getPosition(*s_window).x),
			static_cast<float>(sf::Mouse::getPosition(*s_window).y)
		};

		if (mouse.x > getPosition().x &&
			mouse.x < getPosition().x + getSize().x &&
			mouse.y > getPosition().y &&
			mouse.y < getPosition().y + getSize().y)
		{
			if (!m_isHovered && m_callbacks.find(Event::Hover) != m_callbacks.end())
			{
				m_isHovered = true;
				m_callbacks[Event::Hover]();
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (!m_isClicked && m_callbacks.find(Event::Click) != m_callbacks.end())
				{
					m_isClicked = true;
					m_callbacks[Event::Click]();
				}
			}
			else
				m_isClicked = false;
		}
		else if (m_isHovered)
        {
            m_isHovered = false;

            if (m_callbacks.find(Event::HoverOut) != m_callbacks.end())
				m_callbacks[Event::HoverOut]();
        }

        // also update its children
        for (Widget* w : m_children)
            w->update();
	}

	void on(Event event, const std::function<void()>& callback)
	{
		m_callbacks[event] = callback;
	}

	const sf::Vector2f getPosition() const
	{
	    // can't return reference, because temporary if inherits
	    // returns absolute position, calculated from parent's
	    if (getParent())
            return getParent()->sf::Transformable::getPosition() + sf::Transformable::getPosition();
        else
            return sf::Transformable::getPosition();
	}
	const sf::Vector2f& getSize() const { return m_size; }
	const sf::Color& getColor() const { return m_vertices[0].color; }
	bool getIsVisible() const { return m_isVisible; }
	const Widget* getParent() const { return m_parent; }

	void setColor(const sf::Color& newColor)
	{
		for (sf::Vertex& v : m_vertices)
            v.color = newColor;
	}

	void setIsVisible(bool newIsVisible)
	{
		m_isVisible = newIsVisible;

		// children inherit visibility property
		for (Widget* w : m_children)
			w->setIsVisible(newIsVisible);
	}

	void setParent(Widget* newParent)
	{
		if (!m_parent && newParent) // once the widget receives a good parent, locks the property
        {
            m_parent = newParent;
            m_parent->addChild(this);
        }
	}

protected:
	// attributes
	sf::Vector2f m_size;
	bool m_isVisible;
	sf::Vertex m_vertices[4];
	std::vector<Widget*> m_children;
	Widget* m_parent = nullptr;

	// events
	std::map<Event, std::function<void()>> m_callbacks;
	bool m_isHovered = false;
	bool m_isClicked = false;

	void addChild(Widget* child)
	{
	    TRACK
		m_children.push_back(child);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (getIsVisible())
		{
			states.transform *= getTransform();
     		target.draw(m_vertices, 4, sf::PrimitiveType::Quads, states);

     		// draw its children
     		for (const Widget* w : m_children)
                target.draw(*w, states);
		}
	}
};
