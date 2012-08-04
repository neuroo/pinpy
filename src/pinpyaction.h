/*
    PinPy -- Shake your hooks with some Python!
    by Romain Gaucher <r@rgaucher.info> - http://rgaucher.info

    Copyright (c) 2011-2012 Romain Gaucher     @rgaucher
                            Haris Andrianakis  @tl0gic

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#ifndef PINACTION_H
#define PINACTION_H

#include <string>
#include <map>

namespace pinpy {

// Types of actions supported by the current model
enum ActionType {
    BEFORE_CALL,
    AFTER_CALL,
    REPLACE_CALL,
    NIL_ACTION
};

struct PinPyCall {
    std::string module;
    std::string function;
    std::string message;
    
};

// PinAction contains the information to perform one of the 
// actions delcared in the XML definition.
// Example of actions: log, custom script call, etc.
struct PinPyAction {
    unsigned int id;
    std::string name;
    
    // Python call information
    PinPyCall pyCall;
    
    // Need to hold a pointer to the 'execution driver'
    // PythonCaller *pyScript;
};


}

#endif