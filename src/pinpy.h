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
#ifndef PINPY_H
#define PINPY_H

#include <string>
#include <vector>
#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include <pin.H>

#include "pinfunction.h"
#include "knowledge/knowledge.h"
#include "pin/pintool.h"
#include "python/pyhelper.h"

#define DEBUG_PINTOOL 1

namespace pinpy {

class PinPyModel {
    bool failure;
    std::string confFile;
    std::string pythonPath;
    
    std::vector<PinFunction *> functions;
    std::map<unsigned int, PinFunction const*> functionsHash; 

    // Handle of pin/python
    pintool::PinHelper pinHelper;
    py::PyHelper pyHelper;
    knowledge::Knowledge mining;

    PinPyModel(const PinPyModel&) {}
    PinPyModel& operator=(const PinPyModel& ) {
        return *this;
    }
public:
    PinPyModel() : failure(false) {}
    
    inline bool failed () const {return failure;} 
    bool load(const std::map<std::string, std::string>&);
    
    ~PinPyModel() {
        for (std::vector<PinFunction *>::iterator iter=functions.begin(); iter!=functions.end(); ++iter)
            if (*iter)
                delete *iter;
    }

    py::PyHelper& getPyHelper() {
        return pyHelper;
    }

    inline std::vector<PinFunction *>& getFunctions() {
        return functions;
    }
    
    knowledge::Knowledge& getKnowledge() {
        return mining;
    }
    
    // Prepare python scripts
    void preparePythonScripts(const std::string& paths="");
    
    // Execute PIN probes
    void hookFunctions();
    
public:
    // Testing interfaces
    void runPython(const std::string& module, const std::string& fct);
    
private:
    void walk(xmlNode *);
    PinFunction processPinPyFunction(xmlNode *);
    PinPyAction processPinPyAction(xmlNode *);
    PinParameter processReturnType(xmlNode *);
    PinParameter processFunctionParameter(xmlNode *);
    std::vector<PinParameter> processFunctionParameters(xmlNode *);
    PinFunctionPrototype processPinFunctionPrototype(xmlNode *);

    // Consolidate references in the model
    void consolidateModel();

};


}

#endif
