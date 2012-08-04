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
#ifndef PINFUNCTION_H
#define PINFUNCTION_H

#include <string>
#include <vector>
#include <map>

#include "pinfunctionprototype.h"
#include "pinpyaction.h"

namespace pinpy {

typedef std::vector<PinPyAction> ActionList;

// The PinFunction contains the mapping between
// one function prototype and the several actions 
// applied to it.
class PinFunction {
    std::string name;
    
    // Storage of the prototype for this function
    PinFunctionPrototype proto;
    
    // Contains the list of actions, ordered.
    // e.g., BEFORE_CALL -> [Action_1, Action_2, etc.]
    std::map<ActionType, ActionList> actions;

public:
    PinFunction() {}
    PinFunction(const PinFunction& p) {
        *this = p;
    }
    PinFunction& operator=(const PinFunction& p) {
        name = p.name;
        proto = p.proto;
        actions = p.actions;
        return *this;
    }

    void setPrototype(const PinFunctionPrototype& p) {
        proto = p;
    }
    
    inline const PinFunctionPrototype& getPrototype() {
        return proto;
    }
    
    inline std::map<ActionType, ActionList>& getActions() {
        return actions;
    }
    
    void setAction(ActionType, const PinPyAction&);
    void setAction(const std::string&, const PinPyAction&);
    
    inline void setName(const std::string& str) {
        name = str;
    }
    
    inline std::string getName() const {
        return name;
    }
    
    ~PinFunction() {}
};


}

#endif
