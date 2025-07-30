//
// Created by kalip on 7/28/2025.
//

#pragma once
#include <stack>
#include "State.h"
using namespace std;

class Controller {
    //keep track of curr state and enable going back to prev
    stack<State*> states;
};




