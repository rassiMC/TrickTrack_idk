//Local files
#include "TrackStuff.hpp"
#include "functional.hpp"



//Class TrackSegment

TrackSegment::TrackSegment(std::list<sf::Vector2f> spline_points) {
    degree = size(spline_points) -1;
    spline_points = spline_points;


}

std::list<sf::Vector2f> TrackSegment::get_points() {
    return spline_points;
};

// Class Track

Track::Track() {
    startpoint = sf::Vector2f(0, 0);
    endpoint = sf::Vector2f(0, 0);
}
Track::Track(TrackSegment firstsegment) {
    segments.push_back(firstsegment);
    startpoint = firstsegment.get_points().front();
    endpoint = firstsegment.get_points().back();
}
void Track::add_segment(TrackSegment newsegment) {
    if (newsegment.get_points().front() == startpoint) {
        segments.push_front(newsegment);
        startpoint = newsegment.get_points().front();
    } else if (newsegment.get_points().back() == endpoint) {
        segments.push_back(newsegment);
        endpoint = newsegment.get_points().back();
    }
}
std::list<TrackSegment> Track::get_segments() {
    return segments;
};


//Class TrackLayout
TrackLayout::TrackLayout() {

}

// takes in a track and two points
void TrackLayout::add_Track(Track newtrack) {
    sf::Vector2f startpoint = newtrack.startpoint;
    sf::Vector2f endpoint = newtrack.endpoint;

    Graph::vertex_descriptor start_vertex = find_vertex(startpoint);
    Graph::vertex_descriptor end_vertex = find_vertex(endpoint);
    
    auto edge = boost::add_edge(start_vertex, end_vertex, layout);
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
Graph TrackLayout::get_layout() {
    return layout;
}
;

