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
#include "Python.h"
#include "pin.H"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>

#ifdef _MSC_VER
namespace WINDOWS {
    #include <Windows.h>
}
#else
    #include <dlfcn.h>
#endif

#include "pinpy.h"

using namespace std;
using namespace pinpy;

KNOB<string> knobXMLFile(KNOB_MODE_OVERWRITE, "pintool", "i", "", "XML file containing the actions descriptions");
KNOB<string> knobPythonPath(KNOB_MODE_OVERWRITE, "pintool", "p", "", "Python scripts location (updated python's sys.path)");


void loadPinPy(const map<string, string>& conf, PinPyModel& model) {
    // Load the XML configuration
    if (!model.load(conf)) {
        cerr << "loadPinPy- Failed to load the XML configuration" << endl;
        exit(-1);
    }
    
    map<string, string>::const_iterator inputConfIterator = conf.find("input");
    
    if (inputConfIterator != conf.end())
        cout << "PinPy utility running on " << inputConfIterator->second << endl;
    else {
        cerr << "loadPinPy- Input is not provided" << endl;
        exit(-1);
    }

    
    map<string, string>::const_iterator pythonConfIterator = conf.find("python-path");

    // Prepare python scripts
    if (pythonConfIterator != conf.end())
        model.preparePythonScripts(pythonConfIterator->second);
    else
        model.preparePythonScripts(".");
    
    if (!model.failed()) {
        model.hookFunctions();
    } else {
        cerr << "loadPinPy- Failed to load the model" << endl;
        exit(-1);
    }

#ifdef DEBUG_PINTOOL
    #ifdef _MSC_VER
        WINDOWS::Sleep(10 * 1000);
    #else
        sleep(10);
    #endif
#endif

    // Debug by calling one function that calls ctype for sure.
    // model.runPython("sample", "debug");


    // make symbols available to subsequently loaded .so 
#ifdef _MSC_VER
    WINDOWS::HMODULE h;
    h = WINDOWS::LoadLibraryA("python27.dll");
#else
    void *h;
    h = dlopen("/usr/lib/libpython2.7.so", RTLD_LAZY | RTLD_GLOBAL);
#endif

    if (!h) {
        cerr << "loadPinPy- Failed to load the python library" << endl;
        exit(-1);
    }
}


int Usage() {
    cerr << " PinPy is a pintool tool that aims to replace" << endl;
    cerr << " any actions (aftercall, beforecall, replace) by " << endl;
    cerr << " external python scripts. " << endl;
    cerr << endl << KNOB_BASE::StringKnobSummary() << endl;
    cerr << endl << " For any questions, please contact <lab@cigital.com>" << endl;
    return -1;
}


int main(int argc, char *argv[])
{
    PinPyModel model;

    // Initialize symbol table code, needed for rtn instrumentation
    PIN_InitSymbols();

    // Initialize pin
    if (PIN_Init(argc, argv)) return Usage();

    map<string, string> conf;
    conf["input"] = knobXMLFile.Value();
    if (knobPythonPath.Value().size() > 0)
        conf["python-path"] = knobPythonPath.Value();

    if (conf.find("input") != conf.end())
        loadPinPy(conf, model);
    else {
        Usage();
        return -1;
    }

    // Start the program, never returns
    PIN_StartProgram();

    return 0;
}

