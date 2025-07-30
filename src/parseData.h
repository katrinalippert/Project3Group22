//
// Created by kalip on 7/30/2025.
//

#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <sstream>
#include <iostream>

//for random number generator, used: https://en.cppreference.com/w/cpp/numeric/random/rand.html
#include <ctime>
#include <cstdlib>

using namespace std;

class ParseData{
  //to sort weights, store with id (id, weight)
  vector<pair<int,float>> weights;

  //(id, (cals, weight))
  unordered_map<int, pair<float, float>> calWtMap;

  //(id, (company, name)
  unordered_map<int, vector<string>> namesMap;

  public:
    ParseData(){srand(time(0));} //current time

    void readCsv(string path, bool randomWeights){
      ifstream file(path);

      if(!file.is_open()){
        cout << "Error opening file" << endl;
      }

      //ID,Company,name,cals,weight
      string line;
      int i = 0;
      while(getline(file, line)){
        //first line is column headers so skip
          if(i == 0){
            i++;
            continue;
          }
          istringstream in(line);

          pair<int, float> idWt;
          vector<string> names;
          pair<float, float> calWt;

          string company;
          string name;
          string val;

          getline(in, val, ',');
          int id = stoi(val);

          getline(in, company, ',');
          getline(in, name, ',');
          names.push_back(company);
          names.push_back(name);

          namesMap[id] = names;

          //cals
          getline(in, val, ',');
          calWt.first = stof(val);

          //weight
          getline(in, val, ',');
          //random or file-provided weight
          float weight = stof(val);
          if(!randomWeights){
            calWt.second = weight;
          }
          else{
            float randWt = (rand() % 10000) * 0.8;
            calWt.second = randWt;
            weight = randWt;
          }

          calWtMap[id] = calWt;

          //(id, weight)
          idWt.first = id;
          idWt.second = weight;

          weights.push_back(idWt);

      }
    }



};
