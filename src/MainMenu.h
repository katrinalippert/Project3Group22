//
// Created by kalip on 7/28/2025.
//
#include "State.h"

#pragma once


class MainMenu : public State {
  string API_KEY;

  public:
    MainMenu(string apiKey){this->API_KEY = apiKey;};

};




