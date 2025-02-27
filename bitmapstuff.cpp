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

using namespace std;




std::vector<std::vector<float>> Chunk(64, std::vector<float>(64));


float getHeight(sf::Vector2f) {
    return 0;
}