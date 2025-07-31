//
// Created by kalip on 7/28/2025.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include <curl/curl.h>

using namespace std;


int main() {
    cout << "TEST";

    sf::RenderWindow testWindow(sf::VideoMode(500,500), "TEST");
    while (testWindow.isOpen()) {
        
        sf::Event event;

        while (testWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                testWindow.close();
            }
        }


        testWindow.clear(sf::Color::Red);
        testWindow.display();
    }

    return 0;
}