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
#include <queue>
#include <curl/curl.h>
#include <json.hpp>

//for random number generator, used: https://en.cppreference.com/w/cpp/numeric/random/rand.html
#include <cstdlib>

using namespace std;

class ParseData{

  //to sort weights, store with id (id, weight)
  vector<pair<int,float>> weights;

  //(id, (cals, weight))
  unordered_map<int, pair<float, float>> calWtMap;

  //(id, (company, name)
  unordered_map<int, vector<string>> namesMap;

  string api_key = "";

  struct PairComp {
    //ascending order
    bool operator()(const pair<int,float>& p1, const pair<int,float>& p2) {
      return p1.second > p2.second;
    }
  };

  void sortWeights() {
    //O(nlog(n))
    //min heap
    priority_queue<pair<int, float>, vector<pair<int, float>>, PairComp> pq;

    //add (id, weight) to min heap
    for (auto x : weights) {
      pq.push(x);
    }

    //clear weights
    weights = {};

    //add back sorted pairs
    while (!pq.empty()) {
      auto curr = pq.top();
      weights.push_back(curr);
      pq.pop();
    }
  }

  //from curl docs: https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
  struct memory {
    char *response;
    size_t size;
  };

  static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    reinterpret_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
  }

  string getName(int id, string apiKey){
    CURL* curl;
    CURLcode res;
    string body;

    string url = "https://api.nal.usda.gov/fdc/v1/food/" + to_string(id) + "?api_key=" + apiKey;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
      //set url for get request
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_CAINFO, "dependencies/curl/curl-8.15.0_4-win64-mingw/dep/cacert/cacert.pem");

      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

      res = curl_easy_perform(curl);

      //check if request successful
      if (res != CURLE_OK) {
        cout << "ERROR WITH REQUEST" << endl;
        cout << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        body.clear();
        return "";
      }

      auto bodyJSON = nlohmann::json::parse(body);
      curl_easy_cleanup(curl);
      curl_global_cleanup();

      string name = bodyJSON["description"];

      return name;
    }

    return "";
  }

  public:
    ParseData(){srand(time(0));} //current time

    float getTotalWeight(vector<pair<int,float>> finalWeights) {
      float total = 0;
      for (auto x : finalWeights) {
        total += x.second;
      }
      return total;
    }

    int getTotalItems(vector<pair<int,float>> finalWeights) {
      return finalWeights.size();
    }

    void printSizes() {
      cout << "NAMES MAP: " << namesMap.size() << endl;
      cout << "CAL WEIGHT: " << calWtMap.size() << endl;
      cout << "WEIGHT VECTOR: " << weights.size() << endl;
    }

    void resetParser() {
      weights.clear();
      namesMap.clear();
      calWtMap.clear();
      api_key.clear();
    }


  //DEBUGGING ONLY==================================================================
    void printWeights() {
      for (auto x : weights) {
        cout << "ID: " << x.first << " " << "WEIGHT: "<< x.second << endl;
      }
    }


    void printMaps() {
      //print cals, weights
      for (auto it = calWtMap.begin(); it != calWtMap.end(); it++) {
        cout << "===============================" << endl;
        cout << "ID: " << it->first << endl;
        cout << "CALS: " << it->second.first << endl;
        cout << "WEIGHT: " << it->second.second << endl;
        cout << "===============================" << endl;
      }

      for (auto it = namesMap.begin(); it != namesMap.end(); it++) {
        cout << "++++++++++++++++++++++++++++++" << endl;
        cout << "ID: " << it->first << endl;
        cout << "Company: " << it->second[0] << endl;
        if (it->second.size() > 1) {
          cout << "Name: " << it->second[1] << endl;
        }
        cout << "++++++++++++++++++++++++++++++" << endl;
      }
    }
//================================================================================

    void readCsv(string path, bool randomWeights){
      ifstream file(path);

      if(!file.is_open()){
        cout << "Error opening file" << endl;
      }

      //ID,Company,cals,weight
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
          string val;

          getline(in, val, ',');
          int id = stoi(val);

          //actual product name added later by api request; if response not valid default to company name

          //check if field is quoted and if not read like normal with , delimiter
          //otherwise ignore the open quote, read up to closing quote, and ignore comma
          istringstream testIss(line);
          string idBuffer;
          getline(testIss, idBuffer, ',');
          string companyTest;
          getline(testIss, companyTest, ',');

          if (companyTest[0] != '"') {
            getline(in, company, ',');
          }
          else {
            in.ignore();
            getline(in, company, '"');
            in.ignore();
          }



          names.push_back(company);

          namesMap[id] = names;

          //get calories from file otherwise if not present just use random value
          getline(in, val, ',');
          try {
            calWt.first = stof(val);
          }
          catch (invalid_argument e) {
            float randCal = (rand() % 950) * 0.97;
            calWt.first = randCal;
          }

          //weight
          getline(in, val, ',');
          //random or file-provided weight
          float weight;
          try {
            weight = stof(val);
          }
          catch (invalid_argument e) {
          cout << "++++++++++++++++++++++++" << endl;
          cout << "TRIED : " << val << endl;
          cout << "ID: " << id << endl;
          cout << "++++++++++++++++++++++++" << endl;
        }

          if(!randomWeights){
            calWt.second = weight;
          }
          else{
            float randWt = (rand() % 7500) * 0.8;
            calWt.second = randWt;
            weight = randWt;
          }

          calWtMap[id] = calWt;

          //(id, weight)
          idWt.first = id;
          idWt.second = weight;

          weights.push_back(idWt);
      }

      sortWeights();
    }


    void setNames(vector<int> ids, string apiKey){
       for(auto x : ids) {
         string name = getName(x, apiKey);
         if (!name.empty()) {
           namesMap[x].push_back(name);
         }
       }

       cout << namesMap[1105904][1] << endl;
       cout << namesMap[1105905][1] << endl;
    }

    void setApiKey(string apiKey) {
      api_key = apiKey;
    }

    bool testApiKey(string apiKey) {
      //for testing if user's api key is valid
      string testResult = "";
      try {
        string testResult = getName(1105904, apiKey);
        cout << "RESULT : " << testResult << endl;
      }
      catch (nlohmann::json::exception e) {
        return false;
      }
      //getName returns empty string if unsuccessful so if test result is empty user api key invalid
      return true;
    }

    void createItemCsv(vector<pair<int, float>> finalWeights, int choice) {
      string path = "greedyItemsChosen.txt";
      //1 = dynamic
      if (choice == 1) {
        path = "dynamicItemsChosen.txt";
      }

      ofstream outputFile(path);
      if (outputFile.is_open()) {

      }
    }
};
