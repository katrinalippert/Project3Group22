//
// Created by kalip on 7/28/2025.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "parseData.h"
#include <curl/curl.h>
#include "UI.h"
#include <chrono>

using namespace std;


int main() {
    cout << "TEST" << endl;

    ParseData test;

    //
    auto start = std::chrono::system_clock::now();
    test.readCsv("food_data.csv", false);
    // test.printMaps();
    // test.printSizes();
    auto end = std::chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end-start);

    cout << "DURATION: " << duration.count() << endl;


    test.printSizes();

    // //
    // test.setNames({1105904, 1105905}, apiKey);
    // //
    // test.printMaps();

    // UI testUI;
    // testUI.StartUI();



    return 0;
}