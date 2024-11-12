#ifndef TrackStuff_HPP
#define TrackStuff_HPP

#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>
#include <boost/graph/adjacency_list.hpp>

struct VertexProperties;
struct EdgeProperties;
typedef boost::adjacency_list<
    boost::vecS,                      // OutEdgeList (container for edges)
    boost::vecS,                      // VertexList (container for vertices)
    boost::undirectedS,               // Directed or undirected graph
    VertexProperties,                 // Vertex properties
    EdgeProperties                    // Edge properties
> Graph;




class TrackSegment {
public:
    TrackSegment(std::list<sf::Vector2f> spline_points);
    std::list<sf::Vector2f> get_points();
private:
    int degree;
    std::list<sf::Vector2f> points;
    std::list<double> curvature;
};

class TrackLayout {
public:
    TrackLayout(float radius, float startAngle, float endAngle, unsigned int pointCount);
    void add_Track(TrackSegment newsegment);
private:
    Graph layout;
};

#endif