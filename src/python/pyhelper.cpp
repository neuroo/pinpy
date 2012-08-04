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

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "pyhelper.h"
#include "pinfunctionprototype.h"
#include "utils.h"

using namespace std;
using namespace pinpy;
using namespace utils;

namespace py {

//ofstream out("./python-helper.txt");


PyHelper::PyHelper() 
 : failure(false) {
    Py_Initialize();
}

void PyHelper::initialize() {
    if (!Py_IsInitialized()) {
        failure = true;
    }
}

void PyHelper::setPath(const vector<string>& paths) {
    pythonPaths = paths;
    // Append the paths to the python environment
    if (!Py_IsInitialized()) {
        failure = true;
    }
    else {
        const string sysPath = Py_GetPath();
#ifdef _MSC_VER
        const string pathSeparator = ";";
#else 
        const string pathSeparator = ":";
#endif
        if (paths.size() > 0) {
            finalSysPath = sysPath;
            for (vector<string>::const_iterator iter=paths.begin(); iter!=paths.end(); ++iter) {
                finalSysPath += pathSeparator + *iter;
            }
            if (finalSysPath.size() > 0) {
                PySys_SetPath(const_cast<char *>(finalSysPath.c_str()));
            }
        }
    }
}

void PyHelper::setPath(const string& paths) {
    if (!Py_IsInitialized()) {
        failure = true;
    }
    else {
        // hack to reload the module
        addedPath = paths;
        
        const string sysPath = Py_GetPath();
#ifdef _MSC_VER
        const string pathSeparator = ";";
#else 
        const string pathSeparator = ":";
#endif
        if (paths.size() > 0) {
            finalSysPath = sysPath + pathSeparator + paths;

            if (finalSysPath.size() > 0) {
                PySys_SetPath(const_cast<char *>(finalSysPath.c_str()));
            }
        }
    }
}

void PyHelper::run(const char *module, const char *function, PyObject *dict) const {
    PyObject *pName=0, *pModule=0, *pDict=0, *pFunc=0, *pValue=0, *pArgs=0, *pReloadedModule=0;

    // Build the name object
    pName = PyString_FromString(module);

    // Load the module object
    pModule = PyImport_Import(pName);
    if (!pModule) {
        PyErr_Print();
        if (pName != NULL) {
            Py_DECREF(pName);
        }
        return;
    }
    
    /*ofstream out("./python-relad.txt", ios_base::app);
    out << "Process with loading of function: " << module << "::" << function << flush << endl;
    */
    // pDict is a borrowed reference
    pModule = PyImport_ReloadModule(pModule);
    pDict = PyModule_GetDict(pModule);

    /*
    TODO: debug this routine so that we can dynamically check when the scripts change.
    
    // Check if reload is necessary
    PyObject *moduleFileName = PyDict_GetItemString(pDict, "__file__");
    if (moduleFileName && PyString_Check(moduleFileName)){
        string moduleFileNameString = PyString_AsString(moduleFileName);
        utils::replace(moduleFileNameString, ".pyc", ".py");
        unsigned int h = getFileHash(moduleFileNameString);

        out << "Hash computed for " << moduleFileNameString << " = " << h << endl << flush;

        map<string, unsigned int>::iterator iter = moduleHash.find(moduleFileNameString);
        
        out << "find passed.?" << endl << flush;
        
        if (iter != moduleHash.end()) {
            unsigned int hh = iter->second;
            
            out << "---> " << hh << endl << flush;
            
            if (hh != h) { 
                // reload the module, store the hash of the current file
                pModule = PyImport_ReloadModule(pModule);
                pDict = PyModule_GetDict(pModule);
                moduleHash[moduleFileNameString] = h;
                out << "Changed hash for " << moduleFileNameString << " from " << hh << " to " << h << flush << endl;
            }
        }
        else 
            moduleHash[moduleFileNameString] = h;
    }
    out << flush;
    */
    
    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, function);

    if (pFunc && PyCallable_Check(pFunc)) {
        // Prepare the argument list for the call
        // The only argument we pass is a dictionary object
        if (dict){
            pArgs = PyTuple_New(1);
            PyTuple_SetItem(pArgs, 0, dict);
        }
        // Call the function
        pValue = PyObject_CallObject(pFunc, pArgs);
        if (pArgs != NULL) {
            Py_DECREF(pArgs);
        }

        if (pValue != NULL) {
            // printf("Return of call : %d\n", PyInt_AsLong(pValue));
            Py_DECREF(pValue);
        } else {
            PyErr_Print();
        }
    } else {
        cerr << "PyHelper::run- Failed to load function \"" << function << "\" from module \"" << module << "\"" << endl;
    }

    // Clean up
    Py_DECREF(pModule);
    Py_DECREF(pName);
}

PyHelper::~PyHelper() {
    Py_Finalize();
}   

}
