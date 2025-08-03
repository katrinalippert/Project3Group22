//
// Created by kalip on 7/28/2025.
//

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "parseData.h"
#include "knapsackSolution.h"
#include <chrono>
#include <unordered_map>
#include <sstream>
#include <iomanip>

using namespace std;

struct ResourceManager{
    sf::Font mainFont;
    vector<sf::Texture> textures;
    vector<sf::Sprite> sprites;

    vector<string> optionStrings;
    vector<sf::Text> optionTexts;

    //text object, default string
    unordered_map<string, pair<sf::Text, string>> greedyTexts;
    unordered_map<string, pair<sf::Text, string>> dynamicTexts;


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
      sf::Text greedyCals("TOTAL CALORIES: ", mainFont, 30);
      sf::Text dynamicCals("TOTAL CALORIES: ", mainFont, 30);
      greedyTexts["title"] = make_pair(greedyTitle, greedyTitle.getString());
      greedyTexts["time"] = make_pair(greedyTime, greedyTime.getString());
      greedyTexts["weight"] = make_pair(greedyWeight, greedyWeight.getString());;
      greedyTexts["cals"] = make_pair(greedyCals, greedyCals.getString());

      dynamicTexts["title"] = make_pair(dynamicTitle, dynamicTitle.getString());
      dynamicTexts["time"] = make_pair(dynamicTime, dynamicTime.getString());
      dynamicTexts["weight"] = make_pair(dynamicWeight, dynamicWeight.getString());
      dynamicTexts["cals"] = make_pair(dynamicCals, dynamicCals.getString());


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
        optionStrings.push_back("8. Get Name By ID");

        sf::Text o1,o2,o3,o4,o5,o6,o7,o8;
        optionTexts.push_back(o1);
        optionTexts.push_back(o2);
        optionTexts.push_back(o3);
        optionTexts.push_back(o4);
        optionTexts.push_back(o5);
        optionTexts.push_back(o6);
        optionTexts.push_back(o7);
        optionTexts.push_back(o8);
    }
};

enum class SCREEN{
  OPTIONS,
  INPUT,
  DISPLAY_RESULTS
};

enum class INPUT_MODE{
  ENTER
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

    float greedyTotalCals;
    float dpTotalCals;

    int greedyTotalItems;
    int dpTotalItems;

    Result greedyResult;
    Result dpResult;

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
    sf::Text weightDefaultText;
    sf::Text nameText;


    SCREEN currScreen;
    INPUT_MODE currInputMode;
    int selectedOption;

    public:
      UI(){
        mainWindow.create(sf::VideoMode(800,800), "WINDOW");
        currScreen = SCREEN::OPTIONS;
        currInputMode = INPUT_MODE::ENTER;
        //all options incomplete
        for(int i = 0; i < 8; i++){
          optionStatus.push_back(false);
        }


        for (int i = 0; i < 8; i++) {
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

        nameText.setFont(rm.mainFont);
        nameText.setCharacterSize(40);
        nameText.setPosition(250, 400);
        nameText.setString("PRODUCT NAME: ");


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
        weightDefaultText.setFont(rm.mainFont);

        prompt.setCharacterSize(30);
        resultsDisplay.setCharacterSize(30);
        validationResponse.setCharacterSize(40);
        userInputDisplay.setCharacterSize(40);
        userInputDisplay.setPosition(375, 230);
        userInputDisplay.setString(userInput);
        infoText.setCharacterSize(20);
        weightDefaultText.setCharacterSize(35);


        prompt.setPosition(100,100);
        validationResponse.setPosition(155,280);
        infoText.setPosition(50,700);
        weightDefaultText.setPosition(325, 400);
        weightDefaultText.setString("DEFAULT = 5 KG");

        string info = "Press SPACE to return to Main Menu\nPress R to Reset\nPress ESC to Exit";
        infoText.setString(info);
      }

      void StartUI();

      bool isDpOptimalRatio() {
        //calories/weight
        float greedyRatio = greedyResult.totalCalories / greedyResult.totalWeight;
        float dpRatio = dpResult.totalCalories / dpResult.totalWeight;
        float overall = dpRatio / greedyRatio;
        if (overall < 1) {
          return false;
        }
        return true;
      }

      //press R to reset
      void resetUI(){
        cout << "RESET" << endl;
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

        //set strings to default strings
        for (auto it  = rm.greedyTexts.begin(); it != rm.greedyTexts.end(); it++) {
          it->second.first.setString(it->second.second);
        }
        for (auto it  = rm.dynamicTexts.begin(); it != rm.dynamicTexts.end(); it++) {
          it->second.first.setString(it->second.second);
        }

        cout <<"WEIGHT LIMIT AFTER RESET: " << weightLimit << endl;
        cout << "API KEY AFTER RESET: " << apiKey << endl;
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
            windowTitle.setString("ENTER FOOD CENTRAL API KEY");
            windowTitle.setPosition(270, 140);
            userInput.clear();
            currScreen = SCREEN::INPUT;
            selectedOption = 1;
            // optionStatus[0] = true;
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
              optionStatus[4] = false;
              optionStatus[5] = false;
            }
            parser.readCsv("food_data.csv", false);
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
              optionStatus[4] = false;
              optionStatus[5] = false;
            }
            parser.readCsv("food_data.csv", true);
            // parser.printMaps();
            parser.printSizes();
            optionStatus[2] = true;
          }

          //SET WEIGHT LIMIT
          if (event.key.code == sf::Keyboard::Num4) {
            windowTitle.setString("SET WEIGHT LIMIT");
            userInput.clear();
            currScreen = SCREEN::INPUT;
            optionStatus[3] = true;
            selectedOption = 4;
            //so weight limit not started with 4
            sf::Event bufferEvent;
            while (mainWindow.pollEvent(bufferEvent)) {}
            break;
          }

          if (event.key.code == sf::Keyboard::Num5) {
            //do not run if no dataset loaded
            if (!optionStatus[1] && !optionStatus[2]) {
              continue;
            }
            //run greedy solution
            cout << "GREEDY" << endl;
            auto greedyStart = chrono::high_resolution_clock::now();
            //call greedy
            greedyResult = runKnapsackGreedy(parser.getFoodItems(), weightLimit);
            auto greedyEnd = chrono::high_resolution_clock::now();
            auto greedyDuration = chrono::duration_cast<chrono::microseconds>(greedyEnd - greedyStart);
            greedyMicroSeconds = greedyDuration.count();
            //set total weight (divide by 1000 bc result weight is in grams, convert to kg)
            greedyTotalWeight = greedyResult.totalWeight;
            greedyTotalCals = greedyResult.totalCalories;

            optionStatus[4] = true;
            rm.greedyTexts["title"].first.setString("GREEDY:");
            rm.greedyTexts["title"].first.setFillColor(sf::Color::White);

            string currString = rm.greedyTexts["time"].first.getString();
            ostringstream ss;
            ss << currString << "\n\n" << fixed << setprecision(2) << greedyMicroSeconds/1000 << " ms";
            rm.greedyTexts["time"].first.setString(ss.str());
            //clear ss
            ss.str("");

            string currWeight = rm.greedyTexts["weight"].first.getString();
            ss << currWeight << "\n\n" << fixed << setprecision(2) << greedyTotalWeight;
            rm.greedyTexts["weight"].first.setString(ss.str());
            ss.str("");

            string currCals = rm.greedyTexts["cals"].first.getString();
            ss << currCals << "\n\n" << fixed << setprecision(2) << greedyTotalCals;
            rm.greedyTexts["cals"].first.setString(ss.str());
          }

          if (event.key.code == sf::Keyboard::Num6) {
            //do not run if no dataset
            if (!optionStatus[1] && !optionStatus[2]) {
              continue;
            }
            //run dp solution
            cout << "DYNAMIC" << endl;
            auto dpStart = chrono::high_resolution_clock::now();
            //call dp
            //weight limit * 1000 bc need to convert to grams
            int limitKG = static_cast<int>(weightLimit);
            dpResult = runKnapsackDP(parser.getFoodItems(), limitKG);

            auto dpEnd = chrono::high_resolution_clock::now();
            auto dpDuration = chrono::duration_cast<chrono::microseconds>(dpEnd - dpStart);
            dynamicMicroSeconds = dpDuration.count();
            //weights in grams in csv so divide by 1000 when displaying as kg
            dpTotalWeight = dpResult.totalWeight;
            dpTotalCals = dpResult.totalCalories;

            optionStatus[5] = true;
            rm.dynamicTexts["title"].first.setString("DYNAMIC:");
            rm.dynamicTexts["title"].first.setFillColor(sf::Color::White);

            string currString = rm.dynamicTexts["time"].first.getString();
            ostringstream ss;
            ss << currString << "\n\n" << fixed << setprecision(2) <<  dynamicMicroSeconds/1000 << "  ms";
            rm.dynamicTexts["time"].first.setString(ss.str());
            ss.str("");

            string currWeight = rm.dynamicTexts["weight"].first.getString();
            ss << currWeight << "\n\n" << fixed << setprecision(2) << dpTotalWeight;
            rm.dynamicTexts["weight"].first.setString(ss.str());
            ss.str("");

            string currCals = rm.dynamicTexts["cals"].first.getString();
            ss << currCals << "\n\n" << fixed << setprecision(2) << dpTotalCals;
            rm.dynamicTexts["cals"].first.setString(ss.str());
          }
        }

        if (event.key.code == sf::Keyboard::Num7) {
          //go to results display
          optionStatus[6] = true;
          currScreen = SCREEN::DISPLAY_RESULTS;
        }
        if (event.key.code == sf::Keyboard::Num8) {
          windowTitle.setString("GET PRODUCT NAME FROM 7 DIGIT PRODUCT ID");
          windowTitle.setPosition(210,140);
          optionStatus[7] = true;
          currScreen = SCREEN::INPUT;
          selectedOption = 8;
          sf::Event bufferEvent;
          while (mainWindow.pollEvent(bufferEvent)) {}
          break;
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
              //if key invalid clear user input and allow retry but if valid set the key and complete option
              bool works = parser.testApiKey(userInput);
              if (!works) {
                cout << "INVALID API KEY" << endl;
                userInput.clear();
                validKey = false;
                continue;
              }
              optionStatus[0] = true;
              validKey = true;
              apiKey = userInput;
              currScreen = SCREEN::OPTIONS;
            }
            if (event.key.code == sf::Keyboard::BackSpace) {
              if (!userInput.empty()) {
                userInput.pop_back();
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
            userInputDisplay.setString(userInput);
          }
        }

        if (selectedOption == 4) {
          // cout << "ENTERING LIMIT" << endl;
          windowTitle.setString("SET WEIGHT LIMIT");
          //press enter to submit input
          if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
              weightLimit = stof(userInput);
              cout << "WEIGHT LIMIT: " << weightLimit << endl;
              currScreen = SCREEN::OPTIONS;
            }
            //delete last character of string
            if (event.key.code == sf::Keyboard::BackSpace) {
              if (!userInput.empty()) {
                userInput.pop_back();
              }
            }
          }
          else if (event.type == sf::Event::TextEntered && currInputMode == INPUT_MODE::ENTER) {
            //must be digit for weight limit
            if (event.text.unicode >= '0' && event.text.unicode <= '9' || event.text.unicode == 46) {
              userInput += static_cast<char>(event.text.unicode);
            }
          }
          userInputDisplay.setString(userInput);
          currInputMode = INPUT_MODE::ENTER;
        }

        if (selectedOption == 8) {
            if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::Enter) {
                if (userInput.length() == 7) {
                  string res = "";
                  try {
                    res = parser.getName(stoi(userInput), apiKey);
                  }
                  catch (nlohmann::json::exception e) {
                    res = "CANNOT OBTAIN PRODUCT NAME (INVALID ID)";
                  }

                  string company = parser.getCompanyName(stoi(userInput));
                  nameText.setString("PRODUCT NAME: \n" + company + "\n"  + res);
                }
              }
              else if (event.key.code == sf::Keyboard::BackSpace) {
                if (!userInput.empty()) {
                  userInput.pop_back();
                }
              }
            }
          else if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode >= '0' && event.text.unicode <= '9' && userInput.length() < 7) {
              userInput += static_cast<char>(event.text.unicode);
            }
          }
          userInputDisplay.setString(userInput);
        }
      }
      else if (currScreen == SCREEN::DISPLAY_RESULTS) {
        //if both greedy and dp run then can get files of items chosen
          if (optionStatus[4] && optionStatus[5]) {
            if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::W) {
                //create greedy and dynamic txt files listing items chosen

                  ofstream outFileGreedy("greedyItemsChosen.txt");
                  ofstream outFileDp("dynamicItemsChosen.txt");
                  //default to product ids
                  //greedy
                  for (int i = 0; i < greedyResult.selectedItems.size(); ++i) {
                    if (i != greedyResult.selectedItems.size() - 1) {
                      outFileGreedy << greedyResult.selectedItems[i].id <<"\n";
                    }
                    else {
                      outFileGreedy << greedyResult.selectedItems[i].id;
                    }
                  }
                  //dynamic
                  for (int i = 0; i < dpResult.selectedItems.size(); ++i) {
                    if (i != dpResult.selectedItems.size() - 1) {
                       outFileDp << dpResult.selectedItems[i].id <<"\n";
                    }
                    else {
                      outFileDp << dpResult.selectedItems[i].id;
                    }
                  }

              }
            }
          }
          //if neither greedy nor dp
          if (!optionStatus[4] && !optionStatus[5]) {
            windowTitle.setString("RESULTS\n\n\n\n");
            resultsDisplay.setString("                          NO RESULTS TO DISPLAY.\nPLEASE RUN GREEDY AND/OR DYNAMIC PROGRAMMING ALGORITHMS.\nNOTE: YOU MUST RUN BOTH ALGORITHMS TO SEE A COMPARISON.");
            resultsDisplay.setPosition(190, 250);
            resultsDisplay.setFillColor(sf::Color(214, 101, 15));
          }
          else {
            windowTitle.setString("RESULTS");
            resultsDisplay.setString("");
            rm.greedyTexts["title"].first.setPosition(50,200);
            rm.greedyTexts["time"].first.setPosition(50,250);
            rm.greedyTexts["weight"].first.setPosition(50,375);
            rm.greedyTexts["cals"].first.setPosition(50, 500);

            rm.dynamicTexts["title"].first.setPosition(625,200);
            rm.dynamicTexts["time"].first.setPosition(625,250);
            rm.dynamicTexts["weight"].first.setPosition(625,375);
            rm.dynamicTexts["cals"].first.setPosition(625, 500);
          }

          windowTitle.setPosition( 350, 140);
      }

  }

  //conditional drawing
    mainWindow.clear(sf::Color(117, 161, 155));
    if (currScreen == SCREEN::DISPLAY_RESULTS) {
      mainWindow.draw(resultsDisplay);
      if (optionStatus[4] || optionStatus[5]) {
        //DISPLAY GREEDY
        if (!optionStatus[4]) {
          rm.greedyTexts["title"].first.setFillColor(sf::Color(245, 107, 27));
          mainWindow.draw(rm.greedyTexts["title"].first);
        }
        if (optionStatus[4]) {
          mainWindow.draw(rm.greedyTexts["title"].first);
          mainWindow.draw(rm.greedyTexts["time"].first);
          mainWindow.draw(rm.greedyTexts["weight"].first);
          mainWindow.draw(rm.greedyTexts["cals"].first);
        }

        //DISPLAY DP
        if (!optionStatus[5]) {
          rm.dynamicTexts["title"].first.setFillColor(sf::Color(245, 107, 27));
          mainWindow.draw(rm.dynamicTexts["title"].first);
        }

        if (optionStatus[5]) {
          mainWindow.draw(rm.dynamicTexts["title"].first);
          mainWindow.draw(rm.dynamicTexts["time"].first);
          mainWindow.draw(rm.dynamicTexts["weight"].first);
          mainWindow.draw(rm.dynamicTexts["cals"].first);
        }

        if (optionStatus[4] && optionStatus[5]) {
          mainWindow.draw(fileOptionText);
        }
      }

    }

    else if (currScreen == SCREEN::INPUT) {
      //API KEY
      if (selectedOption == 1) {
        int stars = userInput.length();
        //hide api key input
        userInputDisplay.setString(string(stars, '*'));
        userInputDisplay.setCharacterSize(50);
        mainWindow.draw(userInputDisplay);

        if (!validKey) {
          validationResponse.setFillColor(sf::Color(214, 101, 15));
          validationResponse.setString(" \t\t\t\t\t\tINVALID API KEY.\nPLEASE ENTER VALID KEY OR RETURN TO MENU.\nNOTE: API KEY NEEDED TO OBTAIN NAMES OF ITEMS CHOSEN\n(DEFAULT IS PRODUCT ID).");
          mainWindow.draw(validationResponse);
        }
      }

      //SETTING WEIGHT LIMIT
      if (selectedOption == 4) {
        userInputDisplay.setString(userInput);
        mainWindow.draw(userInputDisplay);
        mainWindow.draw(unitText);
        mainWindow.draw(weightDefaultText);
      }

      //get name from id
      if (selectedOption == 8) {
        userInputDisplay.setString(userInput);
        mainWindow.draw(userInputDisplay);
        mainWindow.draw(nameText);
      }
    }

    else if (currScreen == SCREEN::OPTIONS) {
      windowTitle.setString("MAIN MENU");
      int  i = 0;
      //view results never green, api key option only green/complete when valid key added
      //color green if completed
      for (auto opt : rm.optionTexts) {
        if (i == 7 && !optionStatus[0]) {
          opt.setFillColor(sf::Color(128,128,128));
        }
        else if (optionStatus[i] && i != 6 && i != 7) {
          opt.setFillColor(sf::Color::Green);
          if (i == 0 && !validKey) {
            opt.setFillColor(sf::Color(237, 137, 7));
          }
          else if (i == 0 && apiKey.empty()) {
            opt.setFillColor(sf::Color::White);
          }
        }
        //if dataset not loaded both greedy and dp options gray/will not run the solutions
        else if (!optionStatus[1] && !optionStatus[2]) {
          if (i == 4 || i == 5) {
            opt.setFillColor(sf::Color(128,128,128));
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



