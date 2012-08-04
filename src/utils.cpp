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
#include <fstream>
#include "utils.h"
using namespace std;

namespace utils {

void replace(string& where, const string& what, const string& by) {
    for (string::size_type  i = where.find(what); i != string::npos; i = where.find(what, i + by.size()))
        where.replace(i, what.size(), by);
}

vector<string> split(const string& str, const string& delimiters) {
    vector<string> tokens;
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
    return tokens;
}

unsigned int hash(const string& str) {
    unsigned int hash = 0;
    unsigned int x = 0;
    for(size_t i = 0; i < str.length(); i++) {
        hash = (hash << 4) + str[i];
        if((x = hash & 0xF0000000L) != 0)
            hash ^= (x >> 24);
        hash &= ~x;
    }
    return hash;
}

bool contains(const string& where, const string& what) {
    return string::npos != where.find(what);
}

unsigned int getFileHash(const string& filename) {
    ifstream myfile(filename.c_str());
    string current, complete;
    if (myfile.is_open()) {
        while (myfile.good()) {
            getline (myfile, current);
            complete += current;
        }
        myfile.close();
    }
    if (complete.size() > 0)
        return hash(complete);
    return 0;
}


}