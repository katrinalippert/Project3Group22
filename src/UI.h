//
// Created by kalip on 7/28/2025.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "parseData.h"
#include <chrono>

using namespace std;

struct ResourceManager{
    sf::Font mainFont;
    vector<sf::Image> images;
    vector<sf::Texture> textures;
    vector<sf::Sprite> sprites;

    vector<string> optionStrings;
    vector<sf::Text> optionTexts;

    map<string, sf::Text> greedyTexts;
    map<string, sf::Text> dynamicTexts;


    ResourceManager(){
      //result texts
      sf::Text greedyTitle("GREEDY: \nN/A", mainFont, 35);
      greedyTitle.setFillColor(sf::Color(245, 107, 27));
      sf::Text dynamicTitle("DYNAMIC: \nN/A", mainFont, 35);
      dynamicTitle.setFillColor(sf::Color(245, 107, 27));
      sf::Text greedyTime("TIME TAKEN: ", mainFont, 30);
      sf::Text dynamicTime("TIME TAKEN: ", mainFont, 30);
      sf::Text greedyWeight("TOTAL WEIGHT(KG): ", mainFont, 30);
      sf::Text dynamicWeight("TOTAL WEIGHT(KG): ", mainFont, 30);
      greedyTexts["title"] = greedyTitle;
      greedyTexts["time"] = greedyTime;
      greedyTexts["weight"] = greedyWeight;

      dynamicTexts["title"] = dynamicTitle;
      dynamicTexts["time"] = dynamicTime;
      dynamicTexts["weight"] = dynamicWeight;



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
    ResourceManager rm;
    ParseData parser;
    //default weight limit = 5 kg
    float weightLimit = 5;
    string apiKey;

    bool validKey = true;

    float greedyMicroSeconds;
    float dynamicMicroSeconds;
    float greedyTotalWeight;
    float dpTotalWeight;
    int greedyTotalItems;
    int dpTotalItems;

    sf::RenderWindow mainWindow;

    vector<bool> optionStatus;

    string userInput;
    string optimal;

    sf::Text mainMenuText;
    sf::Text prompt;
    sf::Text validationResponse;
    sf::Text resultsDisplay;
    sf::Text userInputDisplay;
    sf::Text infoText;
    sf::Text windowTitle;
    sf::Text UIHeader;
    sf::Text unitText;
    sf::Text greedyResultText;
    sf::Text dpResultText;
    sf::Text comparisonText;
    sf::Text fileOptionText;

    SCREEN currScreen;
    INPUT_MODE currInputMode;
    int selectedOption;

    public:
      UI(){
        mainWindow.create(sf::VideoMode(800,800), "WINDOW");
        currScreen = SCREEN::OPTIONS;
        currInputMode = INPUT_MODE::ENTER;
        //all options incomplete
        for(int i = 0; i < 7; i++){
          optionStatus.push_back(false);
        }


        for (int i = 0; i < 7; i++) {
          rm.optionTexts[i].setString(rm.optionStrings[i]);
          rm.optionTexts[i].setFont(rm.mainFont);
          rm.optionTexts[i].setFillColor(sf::Color::White);
          rm.optionTexts[i].setCharacterSize(30);
          rm.optionTexts[i].setPosition(270, 200 + (i * 35));

        }

        //results
        greedyResultText.setFont(rm.mainFont);
        greedyResultText.setCharacterSize(30);
        dpResultText.setFont(rm.mainFont);
        dpResultText.setCharacterSize(30);
        comparisonText.setFont(rm.mainFont);
        comparisonText.setCharacterSize(30);
        comparisonText.setPosition(400,400);
        fileOptionText.setFont(rm.mainFont);
        fileOptionText.setCharacterSize(35);
        fileOptionText.setPosition(200,640);
        fileOptionText.setString("PRESS W TO CREATE FILES LISTING ITEMS CHOSEN");

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

        //unit text for set weight limit window
        unitText.setFont(rm.mainFont);
        unitText.setCharacterSize(40);
        unitText.setString("KG");
        unitText.setPosition(380,300);

        //set fonts
        prompt.setFont(rm.mainFont);
        validationResponse.setFont(rm.mainFont);
        resultsDisplay.setFont(rm.mainFont);
        userInputDisplay.setFont(rm.mainFont);
        infoText.setFont(rm.mainFont);

        prompt.setCharacterSize(30);
        resultsDisplay.setCharacterSize(30);
        validationResponse.setCharacterSize(40);
        userInputDisplay.setCharacterSize(40);
        userInputDisplay.setPosition(375, 230);
        userInputDisplay.setString(userInput);
        infoText.setCharacterSize(20);


        prompt.setPosition(100,100);
        validationResponse.setPosition(155,280);
        infoText.setPosition(50,700);

        string info = "Press SPACE to return to Main Menu\nPress R to Reset\nPress ESC to Exit";
        infoText.setString(info);
      }

      void StartUI();

      //press R to reset
      void resetUI(){
        cout << "RESET CALLED" << endl;
        parser.resetParser();
        for(int i = 0; i < 7; i++){
          optionStatus[i] = false;
        }
        currScreen = SCREEN::OPTIONS;
        currInputMode = INPUT_MODE::ENTER;
        resultsDisplay.setString("");
        windowTitle.setString("MAIN MENU");
        validationResponse.setString("");
        userInput.clear();
        weightLimit = 5;
        apiKey.clear();

        cout <<"WEIGHT LIMIT AFTER RESET: " << weightLimit << endl;
        cout << "API KEY AFTER RESET: " << apiKey << endl;
      }

      string findOptimal() {
        if (dpTotalWeight < greedyTotalWeight && dpTotalWeight < weightLimit) {
          return "DYNAMIC";
        }
        if (greedyTotalWeight < weightLimit && greedyTotalWeight < dpTotalWeight) {
          return "GREEDY";
        }
        return "NEITHER APPROACH\nPRODUCED A\nCORRECT SOLUTION";
      }

};

void UI::StartUI() {
  sf::Event event;
  while (mainWindow.isOpen()) {
    while (mainWindow.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        mainWindow.close();
      }

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
          mainWindow.close();
        }

        //space to return to main menu
        if (event.key.code == sf::Keyboard::Space) {
          currScreen = SCREEN::OPTIONS;
          cout << "SPACE" << endl;
        }

        if (event.key.code == sf::Keyboard::R) {
          resetUI();
        }
      }

     //conditional displays
      if (currScreen == SCREEN::OPTIONS) {
        // cout << "OPTIONS SCREEN " << endl;
        if (event.type == sf::Event::KeyPressed) {
          if (event.key.code == sf::Keyboard::Num1) {
            cout << "API KEY" << endl;
            windowTitle.setString("ENTER FOOD CENTRAL API KEY");
            windowTitle.setPosition(270, 140);
            userInput.clear();
            currScreen = SCREEN::INPUT;
            currInputMode = INPUT_MODE::VALIDATE;
            selectedOption = 1;
            optionStatus[0] = true;
            sf::Event bufferEvent;
            while (mainWindow.pollEvent(bufferEvent)) {}
            break;
          }

          if (event.key.code == sf::Keyboard::Num2) {
            cout << "LOAD NON RANDOM" << endl;
            //if user already loaded dataset with random weights need to reset parser
            //and load dataset again with default weights
            if (optionStatus[2]) {
              parser.resetParser();
              optionStatus[2] = false;
            }
            parser.readCsv("TESTFILE.csv", false);
            parser.printSizes();
            optionStatus[1] = true;
          }

          if (event.key.code == sf::Keyboard::Num3) {
            cout << "LOAD RANDOM" << endl;
            //if user already loaded dataset with default/file-provided weights need to reset parser
            //and load dataset again with random weights
            if (optionStatus[1]) {
              parser.resetParser();
              optionStatus[1] = false;
            }
            parser.readCsv("TESTFILE.csv", false);
            parser.printSizes();
            optionStatus[2] = true;
          }

          //SET WEIGHT LIMIT
          if (event.key.code == sf::Keyboard::Num4) {
            windowTitle.setString("SET WEIGHT LIMIT");
            userInput.clear();
            currScreen = SCREEN::INPUT;
            currInputMode = INPUT_MODE::VALIDATE;
            optionStatus[3] = true;
            selectedOption = 4;
            cout << "USER INPUT BEFORE INPUT WINDOW: " << userInput << endl;
            sf::Event bufferEvent;
            //so weight limit not started with 4
            while (mainWindow.pollEvent(bufferEvent)) {}
            break;
          }

          if (event.key.code == sf::Keyboard::Num5) {
            //run greedy solution
            cout << "GREEDY" << endl;
            auto greedyStart = chrono::high_resolution_clock::now();
            //call greedy
            auto greedyEnd = chrono::high_resolution_clock::now();
            auto greedyDuration = chrono::duration_cast<chrono::microseconds>(greedyEnd - greedyStart);
            greedyMicroSeconds = greedyDuration.count();
            //set total weight
            //final weights is vector<pair<int,float>> (id, weight)
            // greedyTotalWeight = parser.getTotalWeight(finalWeights);

            optionStatus[4] = true;
            rm.greedyTexts["title"].setString("GREEDY:");
            rm.greedyTexts["title"].setFillColor(sf::Color::White);

            string currString = rm.greedyTexts["time"].getString();
            currString += "\n\n" + to_string(greedyMicroSeconds) + "  us";
            rm.greedyTexts["time"].setString(currString);

            string currWeight = rm.greedyTexts["weight"].getString();
            currWeight += "\n\n" + to_string(greedyTotalWeight);
            rm.greedyTexts["weight"].setString(currWeight);
          }

          if (event.key.code == sf::Keyboard::Num6) {
            //run dp solution
            cout << "DYNAMIC" << endl;
            auto dpStart = chrono::high_resolution_clock::now();
            //call dp
            auto dpEnd = chrono::high_resolution_clock::now();
            auto dpDuration = chrono::duration_cast<chrono::microseconds>(dpEnd - dpStart);
            dynamicMicroSeconds = dpDuration.count();
            // dpTotalWeight = parser.getTotalWeight(finalWeights);

            optionStatus[5] = true;
            rm.dynamicTexts["title"].setString("DYNAMIC:");
            rm.dynamicTexts["title"].setFillColor(sf::Color::White);

            string currString = rm.dynamicTexts["time"].getString();
            currString += "\n\n" + to_string(dynamicMicroSeconds) + "  us";
            rm.dynamicTexts["time"].setString(currString);

            string currWeight = rm.dynamicTexts["weight"].getString();
            currWeight += "\n\n" + to_string(dpTotalWeight);
            rm.dynamicTexts["weight"].setString(currWeight);
          }
        }

        if (event.key.code == sf::Keyboard::Num7) {
          //go to results display
          optionStatus[6] = true;
          currScreen = SCREEN::DISPLAY_RESULTS;
          if (optionStatus[4] && optionStatus[5]) {
            optimal = findOptimal();
            float percentageFaster = abs(greedyMicroSeconds/dynamicMicroSeconds);
          }
        }
        userInput.clear();
        userInputDisplay.setString(userInput);
      }
      else if (currScreen == SCREEN::INPUT) {

        // cout << "INPUT SCREEN" << endl;
        if (selectedOption == 1) {
          windowTitle.setString("ENTER FOOD CENTRAL API KEY");
          windowTitle.setPosition(270, 140);
          if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
              apiKey = "";
              cout << "CURRENT API KEY: START" << userInput << "END" << endl;
              //guwqECfPfPCfUowggMM92WndoxcCobetdIF2TEDO
              bool works = parser.testApiKey(userInput);
              if (!works) {
                cout << "INVALID API KEY" << endl;
                userInput.clear();
                validKey = false;
                continue;
              }
              // if (!parser.testApiKey("guwqECfPfPCfUowggMM92WndoxcCobetdIF2TEDO")) {
              //   cout << "INVALID KEY" << endl;
              //   continue;
              // }
              validKey = true;
              apiKey = userInput;
              cout << "API KEY FINAL: " << apiKey << endl;
              currScreen = SCREEN::OPTIONS;
            }
            if (event.key.code == sf::Keyboard::BackSpace) {
              if (!userInput.empty()) {
                userInput.pop_back();
                cout << "INPUT NOW API: " << userInput << endl;
              }
            }
            //enable pasting key
            if (event.key.control && event.key.code == sf::Keyboard::V) {
              sf::String pasted = sf::Clipboard::getString();
              userInput = pasted.toAnsiString();
              break;
            }
          }
          else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode >= 33 && event.text.unicode <= 126 ) {
              userInput += static_cast<char>(event.text.unicode);
            }
            cout << "INPUT NOW API: " << userInput << endl;
            userInputDisplay.setString(userInput);
          }



        }

        if (selectedOption == 4) {
          // cout << "ENTERING LIMIT" << endl;
          windowTitle.setString("SET WEIGHT LIMIT");

          if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
              weightLimit = stof(userInput);
              cout << "WEIGHT LIMIT: " << weightLimit << endl;
              currScreen = SCREEN::OPTIONS;
            }
            //delete last character of string
            if (event.key.code == sf::Keyboard::BackSpace) {
              cout << "BACK SPACE" << endl;
              if (!userInput.empty()) {
                userInput.pop_back();
                cout << "INPUT NOW: " << userInput << endl;
              }
            }
          }
          else if (event.type == sf::Event::TextEntered && currInputMode == INPUT_MODE::ENTER) {
            //must be digit for weight limit
            cout << "ENTERING TEXT" << endl;
            if (event.text.unicode >= '0' && event.text.unicode <= '9') {
              userInput += static_cast<char>(event.text.unicode);
              cout << "INPUT NOW: " << userInput << endl;
            }

          }
          userInputDisplay.setString(userInput);
          currInputMode = INPUT_MODE::ENTER;
        }
      }
      else if (currScreen == SCREEN::DISPLAY_RESULTS) {
          if (optionStatus[4] && optionStatus[5] && validKey) {
            if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::W) {
                cout << "VALID W PRESS " << endl;
                //create greedy and dynamic txt files listing items chosen
                // parser.createItemCsv(finalWeights, 0);
                // parser.createItemCsv(finalWeights, 1);
              }
            }
          }
          if (!optionStatus[4] && !optionStatus[5]) {
            windowTitle.setString("RESULTS\n\n\n\n");
            resultsDisplay.setString("                          NO RESULTS TO DISPLAY.\nPLEASE RUN GREEDY AND/OR DYNAMIC PROGRAMMING ALGORITHMS.\nNOTE: YOU MUST RUN BOTH ALGORITHMS TO SEE A COMPARISON.");
            resultsDisplay.setPosition(190, 250);
            resultsDisplay.setFillColor(sf::Color(214, 101, 15));
          }
          else {
            windowTitle.setString("RESULTS");
            resultsDisplay.setString("");
            rm.greedyTexts["title"].setPosition(50,200);
            rm.greedyTexts["time"].setPosition(50,250);
            rm.greedyTexts["weight"].setPosition(50,375);

            rm.dynamicTexts["title"].setPosition(625,200);
            rm.dynamicTexts["time"].setPosition(625,250);
            rm.dynamicTexts["weight"].setPosition(625,375);
          }

            //show greed
          if (optionStatus[4]) {

          }

          //show dynamic
          if (optionStatus[5]) {

          }

          windowTitle.setPosition( 350, 140);
      }

  }


    mainWindow.clear(sf::Color(117, 161, 155));
    if (currScreen == SCREEN::DISPLAY_RESULTS) {
      mainWindow.draw(resultsDisplay);
      if (optionStatus[4] || optionStatus[5]) {
        //DISPLAY GREEDY
        if (!optionStatus[4]) {
          mainWindow.draw(rm.greedyTexts["title"]);
        }
        if (optionStatus[4]) {
          mainWindow.draw(rm.greedyTexts["title"]);
          mainWindow.draw(rm.greedyTexts["time"]);
          mainWindow.draw(rm.greedyTexts["weight"]);
        }

        if (!optionStatus[5]) {
          mainWindow.draw(rm.dynamicTexts["title"]);
        }

        if (optionStatus[5]) {
          mainWindow.draw(rm.dynamicTexts["title"]);
          mainWindow.draw(rm.dynamicTexts["time"]);
          mainWindow.draw(rm.dynamicTexts["weight"]);
        }

        if (optionStatus[4] && optionStatus[5] && validKey) {
          mainWindow.draw(fileOptionText);
        }
      }

    }

    else if (currScreen == SCREEN::INPUT) {
      if (selectedOption == 1) {
        int stars = userInput.length();
        userInputDisplay.setString(string(stars, '*'));
        userInputDisplay.setCharacterSize(50);
        mainWindow.draw(userInputDisplay);
        if (!validKey) {
          validationResponse.setFillColor(sf::Color(214, 101, 15));
          validationResponse.setString(" \t\t\t\t\t\tINVALID API KEY.\nPLEASE ENTER VALID KEY OR RETURN TO MENU.\nNOTE: API KEY NEEDED TO OBTAIN FILES WITH ITEMS CHOSEN.");
          mainWindow.draw(validationResponse);
        }
      }
      if (selectedOption == 4) {
        userInputDisplay.setString(userInput);
        mainWindow.draw(userInputDisplay);
        mainWindow.draw(unitText);
      }
    }

    else if (currScreen == SCREEN::OPTIONS) {

      windowTitle.setString("MAIN MENU");
      int  i = 0;
      for (auto opt : rm.optionTexts) {
        if (optionStatus[i] && i != 6) {
          opt.setFillColor(sf::Color::Green);
          if (i == 0 && !validKey) {
            opt.setFillColor(sf::Color(235, 176, 82));
          }
        }
        else {
          opt.setFillColor(sf::Color::White);
        }
        mainWindow.draw(opt);
        i++;
      }
    }

    mainWindow.draw(infoText);
    mainWindow.draw(windowTitle);
    mainWindow.draw(UIHeader);
    mainWindow.draw(rm.sprites[0]);

    mainWindow.display();


  }
}



