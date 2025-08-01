//
// Created by kalip on 7/28/2025.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "parseData.h"
#include <curl/curl.h>

using namespace std;


int main() {
    cout << "TEST" << endl;

    ParseData test;

    //
    // test.readCsv("TESTFILE.csv", false);
    // //
    // test.setNames({1105904, 1105905}, apiKey);
    // //
    // test.printMaps();


    sf::RenderWindow testWindow(sf::VideoMode(800,800), "TEST");
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