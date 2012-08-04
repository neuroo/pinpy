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
#include <string>
#include <vector>

#include "pinfunctionprototype.h"
#include "utils.h"
using namespace std;
using namespace pinpy;
using namespace utils;

// TODO: Need to implement the reverse of the updateParameterType method here
//       for now bah.
PinParameter::operator string() const {
    string ret = "";
    return ret;
}

// TODO: This function requires unit testing!
void PinParameter::updateParameterType(const string& typeStr_cst) {
    // Stupid normalization happens here
    string typeStr(typeStr_cst);
    replace(typeStr, "  ", " ");
    replace(typeStr, "* *", "**");
    
    // Let's resume with normal stuff.
    vector<string> typeTokens = split(typeStr, " ");
    
    if (typeTokens.size() < 1)
        return;

    // There is a huge normalization process to be done here...
    // TODO: Geez, just think about "MyClass * const" vs. "const MyClass *"
    //       or references to pointers.. "MyClass*&" even though that's stupid shit 
    //       or the "long long", or "long double" ... the split by space is 
    //       just not workking properly.
    //       Embedding proper parser anyone?
    for (vector<string>::const_iterator iter=typeTokens.begin(); iter!=typeTokens.end(); ++iter) {
        const string& cur = *iter;
        
        if ("const" == cur)
            isConst = true;
        else if ("signed" == cur || "unsigned" == cur)
            rangeDecorator = ("signed" == cur ? D_SIGNED : D_UNSIGNED);
        else if ("**" == cur || "*" == cur || "&" == cur)
            typeDecorator = ("*" == cur ? D_POINTER : ("**" == cur ? D_POINTER_POINTER : D_REFERENCE));
        else {
            if ("int" == cur)         type = T_INT;
            else if ("void" == cur)   type = T_VOID;
            else if ("char" == cur)   type = T_CHAR;
            else if ("short" == cur)  type = T_SHORT;
            else if ("long" == cur)   type = T_LONG;
            else if ("float" == cur)  type = T_FLOAT;
            else if ("double" == cur) type = T_DOUBLE;
        }           
    }
}






