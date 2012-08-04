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
#ifndef PYHELPER_H
#define PYHELPER_H

#include <string>
#include <vector>
#include <map>
#include "pinfunctionprototype.h"

namespace py {
    
class PyHelper {
    bool failure;
    std::string finalSysPath;
    std::map<std::string, unsigned int> moduleHash;
    std::vector<std::string> pythonPaths; 
    std::string addedPath;
    
    PyHelper(const PyHelper&) {}
    PyHelper& operator=(const PyHelper& ) {
        return *this;
    }

public:
    PyHelper();
    
    void initialize();
    
    void setPath(const std::string& paths);
    void setPath(const std::vector<std::string>& paths);

    void run(const char *module, const char *function, PyObject *dict) const;

    inline bool failed() const {
        return failure;
    }

    ~PyHelper();
};

    
}

#endif
