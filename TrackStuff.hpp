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
    TrackSegment(sf::Vector2f startpoint, sf::Vector2f endpoint);
    std::list<sf::Vector2f> get_points() const;
    virtual ~TrackSegment() = default;
    virtual double get_curvature(float t) const = 0;  // Pure virtual function
protected:
    sf::Vector2f startpoint;
    sf::Vector2f endpoint;
    std::list<double> curvature;
};

class SplineSegment : public TrackSegment {
public:
    SplineSegment(sf::Vector2f startpoint, sf::Vector2f endpoint);
    double get_curvature(float t) const override;
private:
    // Add any spline-specific members here
};

class CircleSegment : public TrackSegment {
public:
    CircleSegment(sf::Vector2f startpoint, sf::Vector2f endpoint);
    double get_curvature(float t) const override;
private:
    sf::Vector2f center;
    float radius;
    float start_angle;
    float end_angle;
};

class Track {
public:
    Track();
    Track(std::unique_ptr<TrackSegment> firstsegment);
    Track(Track&& other) noexcept = default;  // Add move constructor
    Track& operator=(Track&& other) noexcept = default;  // Add move assignment
    Track(const Track&) = delete;  // Delete copy constructor
    Track& operator=(const Track&) = delete;  // Delete copy assignment

    std::list<sf::Vector2f> get_snappingpoints() const;
    void add_segment(std::unique_ptr<TrackSegment> newsegment);
    const std::list<std::unique_ptr<TrackSegment>>& get_segments() const;
private:
    std::list<std::unique_ptr<TrackSegment>> segments;
    sf::Vector2f startpoint;
    sf::Vector2f endpoint;
    std::list<sf::Vector2f>snappingpoints;
};

// Define vertex properties
struct VertexProperty {
    sf::Vector2f position;
};

// Change EdgeProperty to store shared_ptr to Track
struct EdgeProperty {
    std::shared_ptr<Track> track;
};

// Define the graph type
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
    VertexProperty, EdgeProperty> Graph;

class TrackLayout {
public:
    TrackLayout();
    void add_Track(Track&& newtrack);
    const Graph& get_layout() const;  // Changed to return const reference
    Graph& get_layout();  // Add non-const version
private:
    std::list<sf::Vector2f> snappingpoints;
    Graph layout;

    Graph::vertex_descriptor find_vertex(sf::Vector2f point);
};

#endif