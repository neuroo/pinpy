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
#ifndef PINFUNCTIONPROTOTYPE_H
#define PINFUNCTIONPROTOTYPE_H

#include <string>
#include <vector>

namespace pinpy {

// Type used: if structure or class, please use VOID
enum PinParameterType {
    T_VOID,
    T_CHAR,
    T_SHORT,
    T_INT,
    T_LONG,
    T_LONG_LONG,
    T_FLOAT,
    T_DOUBLE,
    T_LONG_DOUBLE,
    T_STRING
};

// One of the decorators
enum PinParameterDecorator {
    D_SIGNED,
    D_UNSIGNED,
    D_POINTER,
    D_POINTER_POINTER,
    D_REFERENCE
};

struct PinParameter {
    unsigned short id;
    bool isConst;
    PinParameterType type;
    // rangeDecorator: unsigned/signed
    PinParameterDecorator rangeDecorator;
    // typeDecorator: pointer/reference
    PinParameterDecorator typeDecorator;
    std::string name;
    void *value;

    // Update the content of the structure based on the 
    // supplied string
    void updateParameterType(const std::string&);
    
    // "Serialize" the parameter as a string... More useful than simple
    // operator<< overload
    operator std::string() const;
};


// PinFunction contains the different parameters that describe 
// a function definition.
// The goal is to handle a function prototype at this level.
class PinFunctionPrototype {
    // Proto information
    unsigned int address;
    std::string name;
    std::vector<PinParameter> parameters;
    PinParameter returnType;
    
public:
    PinFunctionPrototype() {}
        
    PinFunctionPrototype(const PinFunctionPrototype& p) {
        *this = p;
    }
    
    PinFunctionPrototype& operator=(const PinFunctionPrototype& p) {
        address = p.address;
        name = p.name;
        parameters = p.parameters;
        returnType = p.returnType;
        return *this;
    }
    
    inline std::string getName() const {
        return name;
    }

    inline const std::vector<PinParameter>& getParameters() const {
        return parameters;
    }

    inline void setName(const std::string& str) {
        name = str;
    }
    
    inline void setAddress(const unsigned int addr) {
        address = addr;
    }
    
    inline void setFunctionParameters(const std::vector<PinParameter>& p) {
        parameters = p;
    }
    
    inline void setReturnType(const PinParameter& p) {
        returnType = p;
    }

    
    ~PinFunctionPrototype() {}
};






}

#endif
