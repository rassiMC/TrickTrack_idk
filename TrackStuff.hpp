#ifndef TrackStuff_HPP
#define TrackStuff_HPP
#include <boost/config.hpp>
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>
#include <boost/graph/adjacency_list.hpp>


    
class TrackSegment {
public:
    TrackSegment(std::list<sf::Vector2f> spline_points);
    std::list<sf::Vector2f> get_points() const;
private:
    int degree;
    std::list<sf::Vector2f> spline_points;
    std::list<double> curvature;
};

class Track {
public:
    Track();
    Track(TrackSegment firstsegment);
    sf::Vector2f startpoint;
    sf::Vector2f endpoint;

    void add_segment(TrackSegment newsegment);
    std::list<TrackSegment> get_segments() const;
private:
    std::list<TrackSegment> segments;
};

// Define vertex properties
struct VertexProperty {
    sf::Vector2f position;
};

// Define edge properties
struct EdgeProperty {
    Track track;
};

// Define the graph type
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
    VertexProperty, EdgeProperty> Graph;

class TrackLayout {
public:
    TrackLayout();
    void add_Track(Track newtrack);
    Graph get_layout();
private:
    Graph layout;

    Graph::vertex_descriptor find_vertex(sf::Vector2f point);
    const Graph& get_layout() const;
};

#endif