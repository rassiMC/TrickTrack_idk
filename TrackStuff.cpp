//Local files
#include "TrackStuff.hpp"
#include "functional.hpp"

// i need to work with 1/r as the curvature because it is easier to work with

//Class TrackSegment

TrackSegment::TrackSegment(sf::Vector2f start, sf::Vector2f end) 
    : startpoint(start), endpoint(end)  // Use initialization list instead of assignment
{
    // No need for assignments in the body
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
    : TrackSegment(startpoint, endpoint){
    // Additional initialization for circle segments
    }

double CircleSegment::get_curvature(float t) const {
    
    return 0.0; // represents a straight line for now
}

// Class Track

Track::Track() {
    startpoint = sf::Vector2f(0, 0);
    endpoint = sf::Vector2f(0, 0);
}

Track::Track(std::unique_ptr<TrackSegment> firstsegment) {
    auto points = firstsegment->get_points();
    startpoint = points.front();
    endpoint = points.back();
    segments.push_back(std::move(firstsegment));
}

void Track::add_segment(std::unique_ptr<TrackSegment> newsegment) {
    if (newsegment->get_points().front() == startpoint) {
        segments.push_front(std::move(newsegment));
        startpoint = segments.front()->get_points().front();
    } else if (newsegment->get_points().back() == endpoint) {
        segments.push_back(std::move(newsegment));
        endpoint = segments.back()->get_points().back();
    }
}

std::list<sf::Vector2f> Track::get_snappingpoints() const {
    return std::list<sf::Vector2f>{startpoint, endpoint};
}

const std::list<std::unique_ptr<TrackSegment>>& Track::get_segments() const {
    return segments;
};

//Class TrackLayout
TrackLayout::TrackLayout() {

}

// takes in a track
void TrackLayout::add_Track(Track&& newtrack) {
    // Get the snapping points from the track
    auto points = newtrack.get_snappingpoints();
    if (points.size() < 2) return;

    // Find or create vertices for the start and end points
    Graph::vertex_descriptor v1 = find_vertex(points.front());
    Graph::vertex_descriptor v2 = find_vertex(points.back());

    // Create a shared_ptr to store the track
    std::shared_ptr<Track> track_ptr = std::make_shared<Track>(std::move(newtrack));

    // Add the edge with the track
    boost::add_edge(v1, v2, EdgeProperty{track_ptr}, layout);
}

Graph::vertex_descriptor TrackLayout::find_vertex(sf::Vector2f point) {
    // Look for existing vertex
    auto vp = boost::vertices(layout);
    for (auto it = vp.first; it != vp.second; ++it) {
        if (length_sqr(layout[*it].position.x - point.x, 
                      layout[*it].position.y - point.y) < 100) {
            return *it;
        }
    }
    Graph::vertex_descriptor new_vertex = boost::add_vertex(layout);
    layout[new_vertex].position = point;
    return new_vertex;
}
const Graph& TrackLayout::get_layout() const {
    return layout;
}

Graph& TrackLayout::get_layout() {
    return layout;
};