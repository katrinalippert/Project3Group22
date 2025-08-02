//
// Created by kalip on 7/28/2025.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "parseData.h"


using namespace std;

struct ResourceManager{
    sf::Font mainFont;
    vector<sf::Image> images;
    vector<sf::Texture> textures;
    vector<sf::Sprite> sprites;

    vector<string> optionStrings;
    vector<sf::Text> optionTexts;


    ResourceManager(){
      //knapsack icon
        sf::Texture iconTexture;
        if (!iconTexture.loadFromFile("resources/images/sack-clipart-xl.png")) {
          cout << "TEXTURE ERROR" << endl;
        }
        textures.push_back(iconTexture);

        sf::Sprite iconSprite;
        iconSprite.setTexture(textures[0]);
        iconSprite.setScale(0.05f, 0.05f);
        sprites.push_back(iconSprite);

        mainFont.loadFromFile("resources/fonts/dumero_rounded/DUMERO-Rounded (1).ttf");
        optionStrings.push_back("1. Provide Food Central API Key\n\n");
        optionStrings.push_back("2. Load Dataset with default weights\n\n");
        optionStrings.push_back("3. Load Dataset with random weights\n\n");
        optionStrings.push_back("4. Set Weight Limit\n\n");
        optionStrings.push_back("5. Run Greedy solution\n\n");
        optionStrings.push_back("6. Run Dynamic Programming solution\n\n");
        optionStrings.push_back("7. View Results");

        sf::Text o1,o2,o3,o4,o5,o6,o7;
        optionTexts.push_back(o1);
        optionTexts.push_back(o2);
        optionTexts.push_back(o3);
        optionTexts.push_back(o4);
        optionTexts.push_back(o5);
        optionTexts.push_back(o6);
        optionTexts.push_back(o7);
    }
};

enum class SCREEN{
  OPTIONS,
  INPUT,
  DISPLAY_RESULTS
};

enum class INPUT_MODE{
  ENTER,
  VALIDATE
};

class UI {
    bool testFunction(int num) {
      if (num % 1 == 0) {
        return true;
      }
      return false;
    }

    ResourceManager rm;
    ParseData parser;

    sf::RenderWindow mainWindow;
    sf::Clock clock;

    vector<bool> optionStatus;

    string userInput;

    sf::Text mainMenuText;
    sf::Text prompt;
    sf::Text validationResponse;
    sf::Text resultsDisplay;
    sf::Text userInputDisplay;
    sf::Text infoText;
    sf::Text windowTitle;
    sf::Text UIHeader;

    SCREEN currScreen;
    INPUT_MODE currInputMode;
    int selectedOption;



    public:
      UI(){
        mainWindow.create(sf::VideoMode(800,800), "WINDOW");
        //all options incomplete
        for(int i = 0; i < 6; i++){
          optionStatus.push_back(false);
        }


        for (int i = 0; i < 7; i++) {
          rm.optionTexts[i].setString(rm.optionStrings[i]);
          rm.optionTexts[i].setFont(rm.mainFont);
          rm.optionTexts[i].setFillColor(sf::Color::White);
          rm.optionTexts[i].setCharacterSize(30);
          rm.optionTexts[i].setPosition(270, 200 + (i * 35));

        }



        //main menu text
        windowTitle.setFont(rm.mainFont);
        windowTitle.setString("MAIN MENU\n\n");
        windowTitle.setCharacterSize(40);
        windowTitle.setPosition( 310, 140);

        //
        UIHeader.setString("KNAPSACK");
        UIHeader.setCharacterSize(50);
        UIHeader.setPosition(10, 10);
        UIHeader.setFont(rm.mainFont);
        UIHeader.setFillColor(sf::Color::White);
        sf::FloatRect headerBounds = UIHeader.getGlobalBounds();
        float headerEnd = headerBounds.left + headerBounds.width;
        rm.sprites[0].setPosition(headerEnd + 10, 10);

        //set fonts
        prompt.setFont(rm.mainFont);
        validationResponse.setFont(rm.mainFont);
        resultsDisplay.setFont(rm.mainFont);
        userInputDisplay.setFont(rm.mainFont);
        infoText.setFont(rm.mainFont);

        prompt.setCharacterSize(30);
        resultsDisplay.setCharacterSize(30);
        validationResponse.setCharacterSize(30);
        userInputDisplay.setCharacterSize(25);
        infoText.setCharacterSize(20);

        prompt.setPosition(100,100);
        validationResponse.setPosition(100,100);
        userInputDisplay.setPosition(100,100);
        userInputDisplay.setPosition(100,300);
        infoText.setPosition(50,700);

        string info = "Press SPACE to return to Main Menu\nPress R to Reset\nPress ESC to Exit";
        // string fontTest = "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z";
        infoText.setString(info);

      }

      void StartUI();

      //press R to reset
      void resetUI(){
        cout << "RESET CALLED" << endl;
        parser.resetParser();
        for(int i = 0; i < 6; i++){
          optionStatus[i] = false;
        }
        currScreen = SCREEN::OPTIONS;
        currInputMode = INPUT_MODE::ENTER;
        resultsDisplay.setString("");
        validationResponse.setString("");
        userInput.clear();
      }


};



void UI::StartUI(){
  sf::Event event;
  while(mainWindow.isOpen()){
    while(mainWindow.pollEvent(event)){
      if(event.type == sf::Event::Closed){
        mainWindow.close();
      }

      if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::R){
          resetUI();
        }
        if(event.key.code == sf::Keyboard::Space){
          //press space to return to main menu/options
          cout << "SPACE" << endl;
          currScreen = SCREEN::OPTIONS;
        }

        //escape to exit
        if (event.key.code == sf::Keyboard::Escape) {
          mainWindow.close();
        }

        switch(currScreen){
          //OPTIONS
          case SCREEN::OPTIONS:
            if (event.type == sf::Event::KeyPressed) {
              //GETTING API KEY
              if (event.key.code == sf::Keyboard::Num1) {
                cout << "API KEY CHOSEN" << endl;
                currScreen = SCREEN::INPUT;
                currInputMode = INPUT_MODE::ENTER;
                userInput.clear();
                selectedOption = 1;
              }

              //LOAD DATASET, NON-RANDOM WEIGHTS
              if (event.key.code == sf::Keyboard::Num2) {
                  // parser.readCsv("food_data.csv", false);
                  optionStatus[1] = true;
              }

              //LOAD DATASET, RANDOM WEIGHTS
              if (event.key.code == sf::Keyboard::Num3) {
                  // parser.readCsv("food_data.csv", true);
                  optionStatus[2] = true;
              }

              //SET WEIGHT LIMIT
              if (event.key.code == sf::Keyboard::Num4) {

              }

              //RUN GREEDY
              if (event.key.code == sf::Keyboard::Num5) {

              }

              //RUN DYNAMIC
              if (event.key.code == sf::Keyboard::Num6) {

              }

              //RESULTS DISPLAY
              if (event.key.code == sf::Keyboard::Num7) {

              }
            }

          //INPUT
        }




      }
    }

    mainWindow.clear(sf::Color(117, 161, 155));
    mainWindow.draw(infoText);
    int  i = 0;
    for (auto opt : rm.optionTexts) {
      if (optionStatus[i]) {
        opt.setFillColor(sf::Color::Red);
      }
      mainWindow.draw(opt);
      i++;
    }
    mainWindow.draw(windowTitle);
    mainWindow.draw(UIHeader);
    mainWindow.draw(rm.sprites[0]);

    mainWindow.display();
  }
}