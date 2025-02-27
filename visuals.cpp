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

//Class DisplaySpline

DisplaySpline::DisplaySpline(std::list<sf::Vector2f> points, unsigned int pointCount) {
    std::cout << "Constructor called with " << points.size() << " points" << std::endl;
    vertexArray = sf::VertexArray(sf::LineStrip, pointCount);  // Initialize with primitive type and size
    std::cout << "After construction: " << vertexArray.getVertexCount() << " vertices" << std::endl;
    
    // Initialize all vertices with a default position and color
    for (size_t i = 0; i < vertexArray.getVertexCount(); ++i) {
        vertexArray[i].position = sf::Vector2f(0, 0);
        vertexArray[i].color = sf::Color::White;
    }

    if (points.empty()) {
        std::cout << "Empty points list, returning" << std::endl;
        return;
    }

    current_points = points;
    vertexArray.setPrimitiveType(sf::LineStrip);  // Initialize primitive type first
    vertexArray.resize(pointCount);  // Now safe to resize

    std::list<sf::Vector2f> distributed_points = distributeEqualDistance(spline_length(points), points);
    if (distributed_points.empty()) {
        return;
    }

    unsigned int actual_points = std::min(pointCount, (unsigned int)distributed_points.size());
    vertexArray.resize(actual_points);

    auto it = distributed_points.begin();
    for (unsigned int i = 0; i < actual_points && it != distributed_points.end(); ++i, ++it) {
        vertexArray[i].position = *it;
        vertexArray[i].color = sf::Color::White;
    }
}

void DisplaySpline::redefine(std::list<sf::Vector2f> points, unsigned int pointCount) {
    if (points.size() < 2) {
        return;
    }

    current_points = points;
    
    // Ensure vertexArray is properly initialized
    if (vertexArray.getVertexCount() == 0) {
        vertexArray = sf::VertexArray(sf::LineStrip, pointCount);
        // Initialize all vertices
        for (size_t i = 0; i < vertexArray.getVertexCount(); ++i) {
            vertexArray[i].position = sf::Vector2f(0, 0);
            vertexArray[i].color = sf::Color::White;
        }
    }

    float length = spline_length(points);
    if (length < 1.0f) {
        return;
    }

    std::list<sf::Vector2f> distributed_points = distributeEqualDistance(length, points);
    if (distributed_points.empty()) {
        return;
    }

    unsigned int actual_points = std::min(pointCount, (unsigned int)distributed_points.size());
    vertexArray.resize(actual_points);

    auto it = distributed_points.begin();
    for (unsigned int i = 0; i < actual_points && it != distributed_points.end(); ++i, ++it) {
        vertexArray[i].position = *it;
        
        float t = static_cast<float>(i) / (actual_points - 1);
        float curvature = calc_curvature(points, t);
        
        if (std::abs(curvature) > .00001f) {
            vertexArray[i].color = sf::Color::Red;
        } else if (i % 2 == 0) {
            vertexArray[i].color = sf::Color::Green;
        } else {
            vertexArray[i].color = sf::Color::White;
        }
    }
}

void DisplaySpline::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // Apply the transform of the shape
    states.transform *= getTransform();
    // Draw the vertex array
    target.draw(vertexArray, states);

};