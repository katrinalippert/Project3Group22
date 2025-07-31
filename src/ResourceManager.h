//
// Created by kalip on 7/28/2025.
//

#pragma once
#include <SFML/Graphics.hpp>



class ResourceManager {
    sf::RenderWindow* mainWindow;
    sf::Font* font;

    public:
      ResourceManager(){
        this->mainWindow = nullptr;
        this->font = nullptr;
      }

     void setWindow(sf::RenderWindow* window){
       this->mainWindow = window;
     }

     void setFont(sf::Font* font){
       this->font = font;
     }
};




