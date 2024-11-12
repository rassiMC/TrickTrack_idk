
#include <list>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <array>



int minimum_distance_per_line = 3; // pixls
int spline_accuracy = 20;
float threshhold = 1;


sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t) {
    return sf::Vector2f(
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y)
    );
}

std::list<sf::Vector2f> calcPointOnSpline(std::list<sf::Vector2f> points, float t) {
    std::list<sf::Vector2f> new_points;
    if (size(points) == 2) {
        std::list<sf::Vector2f>::iterator first = points.begin();
        std::list<sf::Vector2f>::iterator second = points.begin();
        std::advance(second, 1);
        std::list<sf::Vector2f> output;
        output.push_back(lerp(*first, *second, t));
        return output;
    }
    for (int i = 0; i + 1 < size(points); i++) {
        std::list<sf::Vector2f>::iterator first = points.begin();
        std::list<sf::Vector2f>::iterator second = points.begin();
        std::advance(first, i);
        std::advance(second, i + 1);
        new_points.push_back(lerp(*first, *second, t));
    }

    return calcPointOnSpline(new_points, t);

}



float length_sqr(float x, float y) {
    return (x * x)+(y * y);
}

float dist_2f(sf::Vector2f a, sf::Vector2f b) {
    return sqrt(length_sqr(a.x - b.x, a.y - b.y));
}

bool areCollinear(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C) {
    float area = A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
    return std::abs(area) < std::numeric_limits<double>::epsilon();
}

double calc_inverse_radius(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C) {
    if (areCollinear(A, B, C)) {
        return 0.0f;
    }
    // Seitenlängen:
    double a = dist_2f(B, C);
    double b = dist_2f(A, C);
    double c = dist_2f(A, B);

    double s = (a + b + c) / 2;

    double triangle_area = std::sqrt(s * (s - a) * (s - b) * (s - c));

    double Radius = (a * b * c) / (4 * triangle_area);

    return 1.0 / Radius;
}


double calc_curvature(std::list<sf::Vector2f> spline_points, float t) {
    // The number of control points
    size_t n = spline_points.size() - 1;

    // Initialize first and second derivatives
    sf::Vector2f B_prime(0, 0);
    sf::Vector2f B_double_prime(0, 0);

    size_t i = 0;
    for (auto it = spline_points.begin(); it != spline_points.end(); ++it, ++i) {
        sf::Vector2f point = *it;
        if (i < n) { // For first derivative
            auto next_it = std::next(it);
            sf::Vector2f next_point = *next_it;
            sf::Vector2f term = (float)n * (next_point - point);
            float scalar = std::pow(1 - t, n - 1 - i) * std::pow(t, i);
            term.x *= scalar;
            term.y *= scalar;
            B_prime += term;
        }
        if (i < n - 1) { // For second derivative
            auto next_it = std::next(it);
            auto next_next_it = std::next(next_it);
            sf::Vector2f next_point = *next_it;
            sf::Vector2f next_next_point = *next_next_it;
            sf::Vector2f term = (float)(n * (n - 1)) * (next_next_point - 2.0f * next_point + point);
            float scalar = std::pow(1 - t, n - 2 - i) * std::pow(t, i);
            term.x *= scalar;
            term.y *= scalar;
            B_double_prime += term;
        }
    }

    // Calculate the curvature using the formula
    double numerator = std::abs(B_prime.x * B_double_prime.y - B_prime.y * B_double_prime.x);
    double denominator = std::pow(B_prime.x * B_prime.x + B_prime.y * B_prime.y, 1.5);

    if (denominator == 0.0) return 0.0; // Avoid division by zero

    return numerator / denominator;
}

float spline_length(std::list<sf::Vector2f> spline_points) {
    int pointcount = size(spline_points) * spline_accuracy;
    sf::Vector2f previous_point = calcPointOnSpline(spline_points, 0).front();
    sf::Vector2f current_point;
    float t = 0.f;
    float accumulaated_distance = 0.f;
    float delta_t = 1.f / float(pointcount);
    for (int i = 0; i < pointcount; i++) {
        t += delta_t;
        current_point = calcPointOnSpline(spline_points, t).front();
        accumulaated_distance += dist_2f(current_point, previous_point);
        previous_point = current_point; 
    }
    return accumulaated_distance;

}

std::list<sf::Vector2f> distributeEqualDistance(float length_of_spline, std::list<sf::Vector2f> spline_points) {
    int pointcount = 0;
    sf::Vector2f previous_point;
    for (std::list<sf::Vector2f>::iterator it = spline_points.begin(); it != spline_points.end(); ++it) {
        if (*it != spline_points.front()){
            pointcount += dist_2f(*it, previous_point) * spline_accuracy / 100.0;
        }
        previous_point = *it;
    }

    std::list<sf::Vector2f> equal_points;
    sf::Vector2f current_point;
    equal_points.push_front(spline_points.front());
    float distance_delta = length_of_spline / pointcount;
    std::cout << distance_delta << ": distance delta \n";
    float target_distance = 0;
    float t = 0;
    float delta_t;
    float actual_distance = 0;
    float accumulated_distance = 0;
    float last_t = 0;
    
    for (int i = 0; i < length_of_spline / distance_delta; i++){
        target_distance += distance_delta;
        std::cout << "ac_dist: " << actual_distance << " trg_dist: " << target_distance << " t: " << t << '\n';
        last_t = t;
        t = .5;
        delta_t = .25;
        actual_distance = 0;
        while (delta_t > float(threshhold) / 10000.f){
            current_point = calcPointOnSpline(spline_points, t).front();
            actual_distance = dist_2f(equal_points.back(), current_point) + accumulated_distance;
            if (actual_distance < target_distance || last_t > t){
                t = t + delta_t;
                std::cout << '+';
            }else{
                t = t - delta_t;
                std::cout << '-';
            }
            delta_t = delta_t / 2;
        }
        accumulated_distance = actual_distance;
        equal_points.push_back(current_point);
    }
    std::list<sf::Vector2f> mylist = equal_points;

    for (int i; i < size(mylist); i++){
        std::cout << mylist.front().x << ' ' << mylist.front().y << '\n';
        mylist.pop_front();
    }
    return equal_points;
}
