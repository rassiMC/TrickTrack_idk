//GUI
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

#include "GUI.hpp"
#include "visuals.hpp"

bool r_click;
bool r_button_held = false;
bool l_click;
bool l_button_held = false;

sf::Vector2f mousePosF;

static sf::RenderWindow* windowPtr = nullptr;  // Add this at the top with other globals

void setWindow(sf::RenderWindow* window) {
    windowPtr = window;
}

void mouseClicks(){
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && r_button_held == false){
        r_click = true;
        r_button_held = true;
    } else{
        r_click = false;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) == false) {
            r_button_held = false;
        }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && l_button_held == false){
        l_click = true;
        l_button_held = true;
    } else{
        l_click = false;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false) {
            l_button_held = false;
        }
    }

    sf::Vector2i mousePos = getMousePos();
    mousePosF = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));  // Assign to global variable

}

bool get_r_click() {
    return r_click;
}
bool get_l_click() {
    return l_click;
}
sf::Vector2i getMousePos() {
    if (windowPtr) {
        return sf::Mouse::getPosition(*windowPtr);  // Get position relative to window
    }
    return sf::Vector2i(0, 0);
}
sf::Vector2f getMousePosF() {
    if (windowPtr) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*windowPtr);  // Get position relative to window
        return sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
    return sf::Vector2f(0.f, 0.f);
}