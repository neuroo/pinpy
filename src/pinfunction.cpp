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
#include <map>
#include <algorithm>

#include "pinfunction.h"
using namespace std;
using namespace pinpy;


ActionType getActionTypeFromString(const string& cstr) {
    static map<string, ActionType> KNOL_ACTION_TYPE;
    if (0 == KNOL_ACTION_TYPE.size()) {
        KNOL_ACTION_TYPE["beforecall"] = BEFORE_CALL;
        KNOL_ACTION_TYPE["aftercall"] = AFTER_CALL;
        KNOL_ACTION_TYPE["replacecall"] = REPLACE_CALL;
    }
    string cstr_ncst(cstr);
    transform(cstr_ncst.begin(), cstr_ncst.end(), cstr_ncst.begin(), ::tolower);

    if (KNOL_ACTION_TYPE.find(cstr_ncst) != KNOL_ACTION_TYPE.end())
        return KNOL_ACTION_TYPE[cstr_ncst];
        
    // Default: BEFORE_CALL
    return NIL_ACTION;
}


void PinFunction::setAction(ActionType actionType, const PinPyAction& action) {
    if (actions.find(actionType) == actions.end())
        actions[actionType] = ActionList();
        
    // TOOD: Need to enforce the order, later on
    actions[actionType].push_back(action);
}


void PinFunction::setAction(const string& actionTypeStr, const PinPyAction& action) {
    ActionType actionType = getActionTypeFromString(actionTypeStr);
    if (NIL_ACTION != actionType)
        setAction(actionType, action);
}
