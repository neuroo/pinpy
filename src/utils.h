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
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <sstream>

namespace utils {

void replace(std::string&, const std::string&, const std::string&);

template<typename T>
std::string tos(const T& t, std::ios_base& (*f)(std::ios_base&)=std::dec,  std::ios_base& (*c)(std::ios_base&)=std::nouppercase) {  
    std::stringstream ss;
    ss << c << f << t;
    return ss.str();
}

template<typename T>
T sot(const std::string& str) {
    T output;
    std::istringstream ss(str);
    ss >> output;
    return output;
}

bool contains(const std::string&, const std::string&);

std::vector<std::string> split(const std::string&, const std::string&);

unsigned int hash(const std::string&);

unsigned int getFileHash(const std::string&);

}

#endif