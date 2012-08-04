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
#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include "knowledge/callgraph.h"

namespace knowledge {

struct Knowledge {
    
    CallGraph callGraph;
    //ValueStore valueStore;

    Knowledge(){}
    ~Knowledge(){}

private:    
    Knowledge(const Knowledge&) {}
    Knowledge& operator=(const Knowledge&) {
        return *this;
    }
};


}

#endif