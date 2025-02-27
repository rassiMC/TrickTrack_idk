//screen stuff

#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>

#ifdef None
#undef None
#endif

#ifdef Default
#undef Default
#endif

#include <X11/Xlib.h>

#include "visuals.hpp"
#include "functional.hpp"

//common screen variables:
std::list<sf::Vector2f> points;
sf::RenderWindow window(sf::VideoMode(2048, 1024), "Tricky_Tracks");




// functions:
void screenSetup(){
    // Create a window
    sf::RenderWindow window(sf::VideoMode(2048, 1024), "Tricky_Tracks");
    sf::Vector2f circlePos(100.f, 100.f);
    sf::CircleShape shape(100.f);
    sf::Vertex vertex(sf::Vector2f(10.f, 50.f), sf::Color::Red, circlePos);  
    shape.setFillColor(sf::Color::Red);
}

sf::Vector2i getMousePos(){
    return sf::Mouse::getPosition(window);
}

//Classes:

Arc::Arc(float radius, float startAngle, float endAngle, unsigned int pointCount) {

    // Calculate the number of points in the arc
    vertexArray.setPrimitiveType(sf::LineStrip);
    vertexArray.resize(pointCount + 1);

    // Generate the points of the arc
    for (unsigned int i = 0; i <= pointCount; ++i) {
        float angle = startAngle + i * (endAngle - startAngle) / pointCount;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        vertexArray[i].position = sf::Vector2f(x, y);
        vertexArray[i].color = sf::Color::White;
    }
}

void Arc::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // Apply the transform of the shape
    states.transform *= getTransform();
    // Draw the vertex array
    target.draw(vertexArray, states);
};

//Class Spline

Spline::Spline(std::list<sf::Vector2f> points, unsigned int pointCount) {
    
    vertexArray.setPrimitiveType(sf::LineStrip);
    vertexArray.resize(pointCount);

    for (unsigned int i = 0; i < pointCount; i++){
        float t = static_cast<float>(i) / (pointCount - 1);
        
        vertexArray[i].position = calcPointOnSpline(points, t).front();
        vertexArray[i].color = sf::Color::White;

    }
    
}

void Spline::redefine(std::list<sf::Vector2f> points, unsigned int pointCount) {
    std::cout << "test if this happend in redefine_0" << std::endl;

    std::list<sf::Vector2f> points_on_spline = distributeEqualDistance(spline_length(points), points); 
    pointCount = size(points_on_spline);
    
    vertexArray.resize(pointCount);
    std::cout << "test if this happend in redefine_1" << std::endl;
    // happened

    for (unsigned int i = 0; i < pointCount; i++){
        float t = static_cast<float>(i) / (pointCount - 1);
        std::cout << "test if this happend in redefine_"<< i + 2 << std::endl;
        // happened
        if (!points_on_spline.empty()) {
            vertexArray[i].position = points_on_spline.front();
            points_on_spline.pop_front();
        } else {
            std::cerr << "Error: points_on_spline is empty at index " << i << std::endl;
            break;
        }
        std::cout << "test if this happend in redefine_"<< i + 3 << std::endl;
        // didn't happen

        if (i>0 && i<pointCount){
            
            if (abs(calc_curvature(points, t)) > .00001){
                
                std::cout << vertexArray[i].position.x - calcPointOnSpline(points, float(i) / float(pointCount)).front().x << vertexArray[i].position.y - calcPointOnSpline(points, float(i) / float(pointCount)).front().y << "\n";
                vertexArray[i].color = sf::Color::Red;
            } else if(i % 2 == 0){
                vertexArray[i].color = sf::Color::Green;
            } else{
                vertexArray[i].color = sf::Color::White;
            }

        }
    }
}

void Spline::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // Apply the transform of the shape
    states.transform *= getTransform();
    // Draw the vertex array
    target.draw(vertexArray, states);

};