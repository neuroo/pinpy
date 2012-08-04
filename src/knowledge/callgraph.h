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
#ifndef CALLGRAPH_H
#define CALLGRAPH_H

#include <boost/config.hpp>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

namespace knowledge {

struct CallGraphNode {
    unsigned int address;
    std::string name;
    
    CallGraphNode(unsigned int a=0, const std::string& n="")
     : address(a), name(n)
    {}
};

struct CallGraphEdge {
    unsigned int flow;
};

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS> Graph;
//typedef boost::adjacency_matrix<boost::directedS> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor CallGraphNodeId;
typedef boost::graph_traits<Graph>::edge_descriptor   CallGraphEdgeId;
typedef boost::graph_traits<Graph>::vertex_iterator   CallGraphNodeIterator;

// We assume that the vertex_descriptor is an unsigned int, since it should be
// the address in the linked-list.

class CallGraph {
    std::map<unsigned int, unsigned int> vertexCache;
    std::map<unsigned int, CallGraphNode> vertex_info;
    std::map<unsigned int, CallGraphEdge> edge_info;
    
    Graph graph;
    
public:
    CallGraph() {}; 
    CallGraph(const CallGraph& c) {
        *this = c;
    }
    CallGraph& operator=(const CallGraph& c) {
        vertexCache = c.vertexCache;
        vertex_info = c.vertex_info;
        edge_info = c.edge_info;
        graph = c.graph;
        return *this;
    }

    CallGraphNodeId getNode(unsigned int address);
    CallGraphNodeId addNode(const CallGraphNode& n);
    
    bool addEdgeNodeId(const CallGraphNodeId&, const CallGraphNodeId&);
    bool addEdge(unsigned int, unsigned int);
    
    unsigned int count_nodes() const {
        return num_vertices(graph);
    }
    
    unsigned int count_edges() const {
        return num_edges(graph);
    }

    //void write(const std::string& fname);



    ~CallGraph();
};


}

#endif
