//Local files
#include "TrackStuff.hpp"
#include "functional.hpp"

// i need to work with 1/r as the curvature because it is easier to work with

//Class TrackSegment

TrackSegment::TrackSegment(sf::Vector2f startpoint, sf::Vector2f endpoint) {
    startpoint = startpoint;
    endpoint = endpoint;
}

std::list<sf::Vector2f> TrackSegment::get_points() const {
    return std::list<sf::Vector2f>{startpoint, endpoint};
};

// SplineSegment implementation
SplineSegment::SplineSegment(sf::Vector2f startpoint, sf::Vector2f endpoint)
    : TrackSegment(startpoint, endpoint)
{
    // Additional initialization for spline segments
}

double SplineSegment::get_curvature(float t) const {

    return 0.0; // represents a straight line for now
}

// CircleSegment implementation
CircleSegment::CircleSegment(sf::Vector2f startpoint, sf::Vector2f endpoint)
    : TrackSegment(startpoint, endpoint){}

double CircleSegment::get_curvature(float t) const {
    
    return 0.0; // represents a straight line for now
}

// Class Track

Track::Track() {
    startpoint = sf::Vector2f(0, 0);
    endpoint = sf::Vector2f(0, 0);
}

Track::Track(std::unique_ptr<TrackSegment> firstsegment) {
    segments.push_back(std::move(firstsegment));
    startpoint = firstsegment->get_points().front();
    endpoint = firstsegment->get_points().back();
}

void Track::add_segment(std::unique_ptr<TrackSegment> newsegment) {
    if (newsegment->get_points().front() == startpoint) {
        segments.push_front(newsegment);
        startpoint = newsegment->get_points().front();
    } else if (newsegment->get_points().back() == endpoint) {
        segments.push_back(newsegment);
        endpoint = newsegment->get_points().back();
    }
}
std::list<std::unique_ptr<TrackSegment>> Track::get_segments() const {
    return segments;
};



//Class TrackLayout
TrackLayout::TrackLayout() {

}

// takes in a track and two points
void TrackLayout::add_Track(Track newtrack) {
    sf::Vector2f startpoint = newtrack.get_snappingpoints().front();
    sf::Vector2f endpoint = newtrack.get_snappingpoints().back();

    Graph::vertex_descriptor start_vertex = find_vertex(startpoint);
    Graph::vertex_descriptor end_vertex = find_vertex(endpoint);
    
    auto edge_pair = boost::add_edge(start_vertex, end_vertex, layout);
    if (edge_pair.second) {
        layout[edge_pair.first].track = newtrack;
    }
}

Graph::vertex_descriptor TrackLayout::find_vertex(sf::Vector2f point) {
    Graph::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(layout); vi != vi_end; ++vi) {
        if (layout[*vi].position == point) {
            return *vi;
        }
    }
    Graph::vertex_descriptor new_vertex = boost::add_vertex(layout);
    layout[new_vertex].position = point;
    return new_vertex;
}
const Graph& TrackLayout::get_layout() const {
    return layout;
}
Graph TrackLayout::get_layout() {
    return layout;
};