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

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "pinpy.h"
#include "utils.h"
#include "pin/pintool.h"

using namespace std;
using namespace pinpy;

string getContent(xmlNode *a_node) {
    xmlChar *c = xmlNodeGetContent(a_node);
    string ret((char *)c);
    utils::replace(ret, "\n", "");
    utils::replace(ret, "\r", "");
    utils::replace(ret, "\b", "");  
    xmlFree(c);
    return ret;
}

string getChildName(xmlNode *node) {
    string content;
    if (XML_ELEMENT_NODE == node->type)
        content = (char *)node->name;
    else if (XML_TEXT_NODE == node->type)
        content = getContent(node);
    return content;
}

map<string, string> getAttributes(xmlNode *node) {
    if (!node || !node->properties)
        return map<string, string>();
    xmlAttr *curAttr = 0;
    string name, value;
    map<string, string> attributes;
    
    for (curAttr = node->properties; curAttr; curAttr = curAttr->next) {
        if (XML_ATTRIBUTE_NODE == curAttr->type) {
            name = (char *)curAttr->name;
            if (XML_TEXT_NODE == curAttr->children->type)
                value = getChildName(curAttr->children);
            if (!(name.empty() || value.empty())) {
                attributes[name] = value;
            }
            name.clear();
            value.clear();
        }
    }
    return attributes;
}


PinPyAction PinPyModel::processPinPyAction(xmlNode *node) {
    PinPyAction action;
    xmlNode *curNode = node, *iterNode = 0;
    string name, value;
        
    if (!curNode)
        return action;  
    
    map<string, string> pinParameterAttrs = getAttributes(curNode);
    
    for (iterNode = curNode->children; iterNode; iterNode = iterNode->next) {
        if (XML_ELEMENT_NODE == iterNode->type) {
            name = (char *)iterNode->name;
            
            // TODO: We assume only one PyCall per PyAction
            //       but we might combine later...
            if ("PyCall" == name) {
                map<string, string> pyCallAttrs = getAttributes(iterNode);
                
                action.pyCall.module = pyCallAttrs["module"];
                action.pyCall.function = pyCallAttrs["function"];
                if (iterNode->children)
                    action.pyCall.message = getContent(iterNode->children);
            }
            name.clear();
            value.clear();
        }
    }
    
    // Update the action with the proper attributes
    action.id = utils::sot<unsigned int>(pinParameterAttrs["sequence"]);
    action.name = pinParameterAttrs["name"];
    
    cerr << action.id << ") Action::name- " << action.name << endl;
    cerr << "python " << action.pyCall.module << ":" << action.pyCall.function << " <- " << action.pyCall.message << endl; 
    return action;
}

PinParameter PinPyModel::processReturnType(xmlNode *node) {
    PinParameter ret;
    if (node->children)
        ret.updateParameterType(getContent(node->children));
    return ret;
}

PinParameter PinPyModel::processFunctionParameter(xmlNode *node) {
    PinParameter param;
    xmlNode *curNode = 0;
    string name, value;
    
    map<string, string> pinParameterAttrs = getAttributes(node);
    
    if (!node->children)
        return param;
    
    for(curNode = node->children; curNode; curNode = curNode->next) {
        if (XML_ELEMENT_NODE == curNode->type) {
            name = (char *)curNode->name;
            if ("FunctionParameterName" == name) {
                // This field if optional, but useful for meaningful output
                param.name = getChildName(curNode->children);
            }
            else if ("FunctionParameterType") {
                // Actual processing of the parameter type
                value = getChildName(curNode->children);
                param.updateParameterType(value);
            }
            name.clear();
            value.clear();
        }
    }
    
    // Add the attributes to the PinParameter
    param.id = utils::sot<unsigned int>(pinParameterAttrs["id"]);
    
    return param;
}

vector<PinParameter> PinPyModel::processFunctionParameters(xmlNode *node) {
    vector<PinParameter> params;
    xmlNode *curNode = 0;
    string name, value;
    
    if (!node->children)
        return params;
    
    for(curNode = node->children; curNode; curNode = curNode->next) {
        if (XML_ELEMENT_NODE == curNode->type) {
            name = (char *)curNode->name;
            if ("FunctionParameter" == name)
                params.push_back(processFunctionParameter(curNode));
            name.clear();
        }
    }
    return params;
}


PinFunctionPrototype PinPyModel::processPinFunctionPrototype(xmlNode *node) {
    string name, value;
    
    PinFunctionPrototype proto;
    xmlNode *curNode = 0;
    
    for(curNode = node; curNode; curNode = curNode->next) {
        if (XML_ELEMENT_NODE == curNode->type) {
            name = (char *)curNode->name;
            if ("FunctionName" == name)
                proto.setName(getChildName(curNode->children));
            else if ("FunctionAddress" == name)
                proto.setAddress(utils::sot<unsigned int>(getChildName(curNode->children)));
            else if ("FunctionParameters" == name)
                proto.setFunctionParameters(processFunctionParameters(curNode));
            else if ("FunctionReturnType" == name)
                proto.setReturnType(processReturnType(curNode));
            name.clear();       
        }
    }
    
    return proto;
}

PinFunction PinPyModel::processPinPyFunction(xmlNode *node) {
    string name, value;
    map<string, string> pinPyFunctionAttrs = getAttributes(node);
    
    PinFunction pinFunction;
    
    xmlNode *curNode = 0;
    // walk and get the checks
    if (node->children) {
        for (curNode = node->children; curNode; curNode = curNode->next) {
            if (XML_ELEMENT_NODE == curNode->type) {
                name = (char *)curNode->name;
                if ("FunctionPrototype" == name) {
                    // Extract the prototype information
                    pinFunction.setPrototype(processPinFunctionPrototype(curNode->children));
                }
                else if ("BeforeCall" == name || "AfterCall" == name || "ReplaceCall" == name) {
                    // Ouch! BadCode 101 - 
                    if (curNode->children && curNode->children->children) {
                        xmlNode *childrenNode = curNode->children->children;
                        for (;childrenNode; childrenNode = childrenNode->next) {
                            if (childrenNode)
                                pinFunction.setAction(name, processPinPyAction(childrenNode));
                        }
                    }
                }
                name.clear();
                value.clear();
            }
        }
    }
    
    pinFunction.setName(pinPyFunctionAttrs["name"]);
    return pinFunction;
}

// Walking the XML DOM, and process every 'PinPyFunction'
void PinPyModel::walk(xmlNode *node) {
    xmlNode *curNode = 0;
    string name;

    for (curNode = node; curNode; curNode = curNode->next) {
        bool processChildren = true;
        if (XML_ELEMENT_NODE == curNode->type) {
            name = (char *)curNode->name;
            if ("PinPyFunction" == name) {
                functions.push_back(new PinFunction(processPinPyFunction(curNode)));
                processChildren = false;
                name.clear(); 
            }
        }
        if (processChildren)
            walk(curNode->children);
    }
}

void PinPyModel::consolidateModel() {
    for (vector<PinFunction *>::const_iterator iter=functions.begin(); iter != functions.end(); ++iter) {
        unsigned int functionId = utils::hash((*iter)->getName());
        if (functionsHash.find(functionId) == functionsHash.end())
            functionsHash[functionId] = *iter;
    }
}

bool PinPyModel::load(const map<string, string>& conf) {
    map<string, string>::const_iterator inputMapIterator = conf.find("input"), pythonMapIterator = conf.find("python-path");
    
    confFile = inputMapIterator->second;
    if (pythonMapIterator != conf.end())
        pythonPath = pythonMapIterator->second;
    
    xmlDoc *doc = 0;
    xmlNode *root_element = 0;

    doc = xmlReadFile(confFile.c_str(), 0, XML_PARSE_NOBLANKS|XML_PARSE_NOCDATA|XML_PARSE_NONET);
    if (doc == 0) {
        cout << "[ERROR] Could not parse file: " << confFile << endl;
        failure = true;
        return false;
    }
    
    // Let's validate the XML with a provided XSD

    // Parse the content of the XML file, and build the model
    root_element = xmlDocGetRootElement(doc);
    walk(root_element);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    
    // Consolidate the model
    consolidateModel();
    
    // Initialize Python
    pyHelper.initialize();
    
    return true;
}


// Iterate through the the different scripts, and make sure they compile
// properly.
void PinPyModel::preparePythonScripts(const string& paths) {
    pyHelper.setPath(paths);
}

// Intent: testing the pytohn module
void PinPyModel::runPython(const string& module, const string& fct) {
    pyHelper.run(module.c_str(), fct.c_str(), NULL);
}


// Install all the probes, pin will scan the binary to detect the prototypes
// and install the trampolines
void PinPyModel::hookFunctions() {
    // Set the overall PinPyModel communication with static class methods
    // Pin interaction has to be done with C++ static class methods, or C-style
    // functions
    pintool::PinHelper::setPinPyModel(this);
    pintool::PinHelper::setPyHelper(&pyHelper);
    
    // Loop through all the functions and install the hooks
    for (vector<PinFunction *>::iterator iter=functions.begin(); iter!=functions.end(); ++iter) {
        PinFunction* cur = *iter;
        const PinFunctionPrototype& proto = cur->getPrototype();
        cout << "PinPyModel::hookFunctions- Hooking function " << proto.getName() << endl;
        
        // Register Routine to be called to instrument rtn, passing the current
        // PinFunction to the routine
        RTN_AddInstrumentFunction(pintool::PinHelper::routine, (void *)(&(*cur)));
    }

    // Register Fini to be called when the application exits
    PIN_AddFiniFunction(pintool::PinHelper::fini, 0);
}

