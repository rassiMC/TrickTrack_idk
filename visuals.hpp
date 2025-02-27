#ifndef visuals_HPP
#define visuals_HPP

#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>


void screenSetup();
//Class Arc

class Arc : public sf::Drawable, public sf::Transformable {
public:
    Arc(float radius, float startAngle, float endAngle, unsigned int pointCount = 30);
   
private:
    sf::VertexArray vertexArray;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

//Class DisplaySpline

class DisplaySpline : public sf::Drawable, public sf::Transformable {
public:
    DisplaySpline(std::list<sf::Vector2f> points, unsigned int pointCount);

    void redefine(std::list<sf::Vector2f> points, unsigned int pointCount);
private:
    sf::VertexArray vertexArray;
    std::list<sf::Vector2f> current_points;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif