#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>
#include <random>

#ifdef None
#undef None
#endif

#ifdef Default
#undef Default
#endif

#include <X11/Xlib.h>

#include "visuals.hpp"
#include "TrackStuff.hpp"
#include "functional.hpp"

using namespace std;


int LOLOLOL = 6546;


int main() {


        bool r_click;
        bool r_button_held = false;
        bool l_click;
        bool l_button_held = false;
    
    // Create a window
    sf::RenderWindow window(sf::VideoMode(2048, 1024), "Tricky_Tracks");
    sf::Vector2f circlePos(100.f, 100.f);
    sf::CircleShape shape(100.f);
    sf::Vertex vertex(sf::Vector2f(10.f, 50.f), sf::Color::Red, circlePos);  
    shape.setFillColor(sf::Color::Red);


    // Create an arc with a radius of 100 pixels, from 30 to 120 degrees
    Arc arc(100.f, 0.f, 1.f);
    arc.setPosition(400.f, 300.f);

    std::list<sf::Vector2f> points;
    points.push_back(sf::Vector2f(100.f, 500.f));
    points.push_back(sf::Vector2f(200.f, 400.f));

    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range [200, 500)
    std::uniform_real_distribution<float> dist(200.0f, 500.0f);


    std::list<Spline> splines;
    for (unsigned int i; i < 50; i++){
        for (unsigned int u; u < 3 ; u++){
            sf::Vector2f point = sf::Vector2f(dist(gen), dist(gen));
            points.push_back(point);

        }
        splines.push_back(Spline(points, 100));
    }


    Spline spline(points, 100);
    
    int count = 0;
    
  
    points.clear();

    points.push_back(sf::Vector2f(100.f, 500.f));
  
  
  //main Loop


    while (window.isOpen()) {
        sf::Event event;
        r_click = false;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && r_button_held == false) {
            r_click = true;
            r_button_held = true;
        } else{
            r_click = false;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right) == false) {
                r_button_held = false;
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && l_button_held == false) { 
            l_click = true;
            l_button_held = true;
        } else{
            l_click = false;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) {
                l_button_held = false;
            }
        }

        window.clear(sf::Color::Black);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if(length_sqr(mousePosF.x - circlePos.x, mousePosF.y - circlePos.y) < 10000) {
            shape.setFillColor(sf::Color::Green);
            std::cout << "the number is: " << LOLOLOL; 
        }else {
            shape.setFillColor(sf::Color::Blue);
        }

        
        points.push_back(mousePosF);
        window.draw(shape);
        window.draw(arc);  // Draw the arc

        splines.back().redefine(points, 10);
        for (const auto& spline : splines) {
        window.draw(spline);
        }
        if (l_click) {
            splines.push_back(Spline(points, 10));
            points.clear();
            points.push_back(mousePosF);
            
        }else if (not r_click) {
            points.pop_back();
        }

        window.display();

        std::cout << "hier ist das problem nicht";

        count ++;

    }

    return 0;
}



