#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>
#include <boost/graph/adjacency_list.hpp>

//Local files
#include "TrackStuff.hpp"
#include "functional.hpp"

struct VertexProperties {
    sf::Vector2f position;
    int value;
};

struct EdgeProperties {
    TrackSegment Track;
};

//Class TrackSegment

TrackSegment::TrackSegment(std::list<sf::Vector2f> spline_points) {
    degree = size(spline_points) -1;
    points = spline_points;


}

std::list<sf::Vector2f> TrackSegment::get_points() {
    return points;
};



//Class TrackLayout
TrackLayout::TrackLayout(float radius, float startAngle, float endAngle, unsigned int pointCount) {
    
}
void TrackLayout::add_Track(TrackSegment newsegment) {
    auto newPoint = add_vertex(layout);
};