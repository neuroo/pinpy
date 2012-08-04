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

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include "utils.h"
#include "pinpy.h"
#include "pinfunction.h"
#include "pinfunctionprototype.h"
#include "pin/pintool.h"

using namespace pinpy;
using namespace py;
using namespace std;
using namespace utils;

namespace pintool {


pinpy::PinPyModel* PinHelper::pinPyModelStatic = 0;
py::PyHelper* PinHelper::pyHelperStatic = 0;

void PinHelper::afterCall(const char *funcName, const PinPyAction* action, vector<PinParameter> *pinParams, void *retVal, ...)
{
    if (!pyHelperStatic || !action)
        return;

    //cerr << funcName << " -> " << action->pyCall.module.c_str() << ", " << action->pyCall.function.c_str() << endl;

    va_list vl;

    PyObject *python_dict = PyDict_New();
    if (!python_dict) {
        PyErr_Print();
        return;
    }
    
    PyDict_SetItemString(python_dict, "@threadid", PyInt_FromSize_t((size_t)PIN_GetTid()));
    PyDict_SetItemString(python_dict, "@callMode", PyString_FromString("afterCall"));
    PyDict_SetItemString(python_dict, "@funcName", PyString_FromString(funcName));
    PyDict_SetItemString(python_dict, "@numArgs",  PyInt_FromSize_t(pinParams->size()));
    PyDict_SetItemString(python_dict, "@ret",      PyInt_FromSize_t((uintptr_t)retVal));
    
    va_start(vl, retVal);
    for (vector<PinParameter>::iterator iter=pinParams->begin(); iter!=pinParams->end(); ++iter) {
        PinParameter *cur = &(*iter);
        cur->value = va_arg(vl, void*);
        PyDict_SetItemString(python_dict, cur->name.c_str(), PyInt_FromSize_t((uintptr_t)cur->value));
    }
    va_end(vl);

    pyHelperStatic->run(action->pyCall.module.c_str(), action->pyCall.function.c_str(), python_dict);
    Py_DECREF(python_dict);
}

void PinHelper::beforeCall(const char *funcName, const PinPyAction* action, vector<PinParameter> *pinParams, ...)
{
    if (!pyHelperStatic || !action)
        return;
    
    //cerr << funcName << " -> " << action->pyCall.module.c_str() << ", " << action->pyCall.function.c_str() << endl;
    
    va_list vl;

    PyObject *python_dict = PyDict_New();
    if (!python_dict) {
        PyErr_Print();
        return;
    }
    
    PyDict_SetItemString(python_dict, "@threadid", PyInt_FromSize_t((size_t)PIN_GetTid()));
    PyDict_SetItemString(python_dict, "@callMode", PyString_FromString("beforeCall"));
    PyDict_SetItemString(python_dict, "@funcName", PyString_FromString(funcName));
    PyDict_SetItemString(python_dict, "@numArgs",  PyInt_FromSize_t(pinParams->size()));
    
    va_start(vl, pinParams);
    for (vector<PinParameter>::iterator iter=pinParams->begin(); iter!=pinParams->end(); ++iter) {
        PinParameter *cur = &(*iter);
        cur->value = va_arg(vl, void*);
        PyDict_SetItemString(python_dict, cur->name.c_str(), PyInt_FromSize_t((uintptr_t)cur->value));
    }
    va_end(vl);

    pyHelperStatic->run(action->pyCall.module.c_str(), action->pyCall.function.c_str(), python_dict);
    Py_DECREF(python_dict);
}

void PinHelper::setProbe(RTN rtn, IPOINT point, AFUNPTR newfunc, vector<PinParameter> *params, ActionType actionType, const ActionList& actionList)
{
    const char *funcName = RTN_Name(rtn).c_str();
    unsigned int i = 0;
    IARGLIST args = IARGLIST_Alloc();
    
    for (;i < params->size(); i++)
        IARGLIST_AddArguments(args, IARG_FUNCARG_ENTRYPOINT_REFERENCE, i, IARG_END);

    if (actionType == AFTER_CALL) {
        for (ActionList::const_iterator iter=actionList.begin(); iter!=actionList.end(); ++iter) {
            const PinPyAction *actionPtr = &(*iter);
            RTN_InsertCall(rtn, point, (AFUNPTR)newfunc, IARG_PTR, funcName, IARG_PTR, actionPtr, IARG_PTR, params, IARG_FUNCRET_EXITPOINT_REFERENCE, IARG_IARGLIST, args, IARG_END);
        }
    }
    else {
        for (ActionList::const_iterator iter=actionList.begin(); iter!=actionList.end(); ++iter) {
            const PinPyAction *actionPtr = &(*iter);
            RTN_InsertCall(rtn, point, (AFUNPTR)newfunc, IARG_PTR, funcName, IARG_PTR, actionPtr, IARG_PTR, params, IARG_IARGLIST, args, IARG_END);
        }
    }
    IARGLIST_Free(args);
}

ofstream out("./graph.txt");
bool start_tracing = false;

void PinHelper::processKnowledge(RTN rtn, const PinFunction& function, const PinFunctionPrototype& proto) {
    using namespace knowledge;
    

    RTN previousRTN = RTN_Prev(rtn);
    if (previousRTN != RTN_Invalid()/* && pinPyModelStatic != 0*/) {
        const ADDRINT callerAddress = RTN_Address(previousRTN);
        const string callerName = RTN_Name(previousRTN);
        const string name = RTN_Name(rtn);
        const ADDRINT address = RTN_Address(rtn);
        
        //unsigned int currentThread = PIN_GetTid();
        if (start_tracing)
            out << "," << callerName << "," << name << endl << flush;
        
        /*
        CallGraph* callGraph = &(pinPyModelStatic->getKnowledge().callGraph);
        
        if (callGraph) {        
            CallGraphNode caller(callerAddress, callerName);
            CallGraphNode callee(address, name);
    
            out << currentThread << " || found: caller at " << hex << callerAddress  << " (" << callerName << ")" << endl;
    
            CallGraphNodeId callerId = callGraph->addNode(caller);
            CallGraphNodeId calleeId = callGraph->addNode(callee);

            out << currentThread << " || edge(" << callerId << "," << calleeId << ")" << endl;

            if (false == callGraph->addEdgeNodeId(callerId, calleeId)) {
                out << currentThread << " || No idea why I cannot add a node here!" << endl;
                out << currentThread << " || ####### " << callerId << " -> " << calleeId << " failed." << endl; 
            }
            out << flush;
        }
        */
    }
}


// Pin calls this function every time a new rtn is executed
VOID PinHelper::routine(RTN rtn, VOID *v)
{
    if (!RTN_Valid(rtn))
        return; 
    
    PinFunction& function = *((PinFunction *)v);
    const PinFunctionPrototype& proto = function.getPrototype();
    const map<ActionType, ActionList>& actions = function.getActions();
    const vector<PinParameter>& params = proto.getParameters();

    if (RTN_Name(rtn) == proto.getName()) {
        // start_tracing = true;
        RTN_Open(rtn);
        // cout << "Instrumenting function " << RTN_Name(rtn) << " at address " << hex << RTN_Address(rtn) << endl;

        // TODO: implement REPLACE_CALL
        map<ActionType, ActionList>::const_iterator afterCallIter = actions.find(AFTER_CALL);
        map<ActionType, ActionList>::const_iterator beforeCallIter = actions.find(BEFORE_CALL);
        
        // Shall we instrument both before/after calls?
        if (beforeCallIter != actions.end())
            setProbe(rtn, IPOINT_BEFORE, (AFUNPTR)PinHelper::beforeCall, const_cast<vector<PinParameter> *>(&params), BEFORE_CALL, beforeCallIter->second);
        
        if (afterCallIter != actions.end()) 
            setProbe(rtn, IPOINT_AFTER, (AFUNPTR)PinHelper::afterCall, const_cast<vector<PinParameter> *>(&params), AFTER_CALL, afterCallIter->second);
        
        RTN_Close(rtn);
    }
    
    // Process knowledge from current RTN
    // processKnowledge(rtn, function, proto);
}

// This function is called when the application exits
VOID PinHelper::fini(INT32 code, VOID *v)
{
    return;
}

}