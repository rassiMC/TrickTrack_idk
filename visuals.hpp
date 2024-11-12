#ifndef visuals_HPP
#define visuals_HPP

#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>


void screenSetup();
sf::Vector2i getMousePos();
//Class Arc

class Arc : public sf::Drawable, public sf::Transformable {
public:
    Arc(float radius, float startAngle, float endAngle, unsigned int pointCount = 30);
   
private:
    sf::VertexArray vertexArray;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

//Class Spline

class Spline : public sf::Drawable, public sf::Transformable {
public:
    Spline(std::list<sf::Vector2f> points, unsigned int pointCount);

    void redefine(std::list<sf::Vector2f> points, unsigned int pointCount);
private:
    sf::VertexArray vertexArray;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif