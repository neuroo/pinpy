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
#ifndef _PINTOOLH
#define _PINTOOLH

#include <vector>
#include <pin.H>

#include "knowledge/callgraph.h"
#include "pinfunctionprototype.h"
#include "pinpyaction.h"
#include "python/pyhelper.h"

// forward definition of PinPyModel
namespace pinpy {
    class PinPyModel;
}

namespace pintool {

class PinHelper {
    
    PinHelper(const PinHelper&) {}
    PinHelper& operator=(const PinHelper& ) {
        return *this;
    }
public:

    static pinpy::PinPyModel* PinHelper::pinPyModelStatic;
    static py::PyHelper* PinHelper::pyHelperStatic;
    
    PinHelper() {}

    static void setPinPyModel(pinpy::PinPyModel* m) {
        pinPyModelStatic = m;
    }
    
    static void setPyHelper(py::PyHelper* p) {
        pyHelperStatic = p;
    }
    // Fanning place to process any type of analysis
    static void processKnowledge(RTN, const pinpy::PinFunction&, const pinpy::PinFunctionPrototype&);
    
    static void afterCall(const char *funcName, const pinpy::PinPyAction* action, std::vector<pinpy::PinParameter> *pinParams, void *retVal, ...);
    static void beforeCall(const char *funcName, const pinpy::PinPyAction* action,std::vector<pinpy::PinParameter> *pinParams, ...);

    static void setProbe(RTN rtn, IPOINT point, AFUNPTR newfunc, std::vector<pinpy::PinParameter> *params, pinpy::ActionType, const pinpy::ActionList&);

    // Pin calls this function every time a new rtn is executed
    static VOID routine(RTN rtn, VOID *v);

    // This function is called when the application exits
    // It prints the name and count for each procedure
    static VOID fini(INT32 code, VOID *v);

    ~PinHelper(){}
};

}

#endif
