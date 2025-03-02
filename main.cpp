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
#include "GUI.hpp"

using namespace std;


int LOLOLOL = 6546;


int main() {
    
    // Create a window
    sf::RenderWindow window(sf::VideoMode(2048, 1024), "Tricky_Tracks");
    setWindow(&window);  // Add this line after window creation
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

    TrackLayout TA = TrackLayout();

    // Create a list of splines
    std::list<DisplaySpline> splines;

    DisplaySpline spline(points, 100);
    splines.push_back(spline);
    
    int count = 0;
    
  
    points.clear();

    points.push_back(sf::Vector2f(100.f, 500.f));
    sf::Vector2f prev_ancher_point = getMousePosF();
    points.push_back(prev_ancher_point);
  
  

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        mouseClicks();
        bool l_click = get_l_click();
        bool r_click = get_r_click();
        sf::Vector2f mousePosF = getMousePosF();

        window.clear(sf::Color::Black);

        if(length_sqr(mousePosF.x - circlePos.x, mousePosF.y - circlePos.y) < 10000) {
            shape.setFillColor(sf::Color::Green);
            std::cout << "the number is: " << LOLOLOL; 
        }else {
            shape.setFillColor(sf::Color::Blue);
            std::cout << "mouse position: " << mousePosF.x << " " << mousePosF.y << std::endl;
        }
        
        window.draw(shape);
        window.draw(arc);
        //splines.back().redefine(points, 100);

        for (const auto& spline : splines) {
            window.draw(spline);
        }
        
        auto edge_range = boost::edges(TA.get_layout());
        for (auto it = edge_range.first; it != edge_range.second; ++it) {
            const Track& track = *(TA.get_layout()[*it].track);
            for (const auto& segment : track.get_segments()) {
                DisplaySpline spline(segment->get_points(), 1000);
                window.draw(spline);
            }
        }
        points.push_back(prev_ancher_point);
        points.push_back(mousePosF);
        splines.pop_back();
        splines.push_back(DisplaySpline(points, 500));

        if (l_click) {
            std::cout << "Left click detected" << std::endl;
            sf::Vector2f start = points.front();
            sf::Vector2f end = points.back();
            Track track(std::make_unique<CircleSegment>(start, end));
            TA.add_Track(std::move(track));
            prev_ancher_point = mousePosF;
        }
        points.clear();
        window.display();

        count ++;
    }
    return 0;
}