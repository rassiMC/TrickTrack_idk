#include <list>
#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>

sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t);

std::list<sf::Vector2f> calcPointOnSpline(std::list<sf::Vector2f> points, float t);

float spline_length(std::list<sf::Vector2f> spline_points);

float length_sqr(float x, float y);
sf::Vector2f dist_2f(sf::Vector2f a, sf::Vector2f b);
bool areCollinear(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C);
float calc_inverse_radius(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C);

double calc_curvature(std::list<sf::Vector2f> spline_points, float t);

std::list<sf::Vector2f> distributeEqualDistance(float length_of_spline, std::list<sf::Vector2f> spline_points);