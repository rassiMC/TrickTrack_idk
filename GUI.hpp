#pragma once
#include <SFML/Graphics.hpp>

void mouseClicks();
bool get_r_click();
bool get_l_click();
sf::Vector2i getMousePos();
sf::Vector2f getMousePosF();
void setWindow(sf::RenderWindow* window);