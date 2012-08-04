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
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

#include "knowledge/callgraph.h"
using namespace boost;
using namespace std;
using namespace knowledge;

ofstream out("graph-internal.txt");

CallGraphNodeId CallGraph::addNode(const CallGraphNode& n) {
    CallGraphNodeId nId = getNode(n.address);
    if (nId == Graph::null_vertex()) {
        CallGraphNodeId nId2 = add_vertex(graph);
        // vertex_info[reinterpret_cast<unsigned int>(nId2)] = n;
        //vertexCache[n.address] = nId2;
        out << "Add new vertex: " << nId2 << " address =" << n.address << " (V=" << num_vertices(graph) << ")" << endl;
        return nId2;
    }
    else {
        out << "Existing vertex for address = "  << " id=" << dec << nId << endl;
    }
    return nId;
}

CallGraphNodeId CallGraph::getNode(unsigned int address) {
    if (vertexCache.size() < 1)
        return Graph::null_vertex();
    
    /*
    map<unsigned int, unsigned int>::iterator cacheIterator = vertexCache.find(address);
    if (cacheIterator != vertexCache.end())
        return cacheIterator->second != Graph::null_vertex() ? cacheIterator->second : Graph::null_vertex();
    */
    return Graph::null_vertex();
}

bool CallGraph::addEdgeNodeId(const CallGraphNodeId& n1, const CallGraphNodeId& n2) {
    if (n1 == Graph::null_vertex() || n2 == Graph::null_vertex())
        return false;
    pair<CallGraphEdgeId, bool> edgePair = edge(n1, n2, graph); 
    if (!edgePair.second) {
        // Need to add the edge
        pair<CallGraphEdgeId, bool> e = add_edge(n1, n2, graph);
        out << "new edge- " << e.first << " (E=" << dec << num_edges(graph) << ")" << flush << endl;
        if (e.second) {
            //graph[e.first].flow = 1;
        }
        return e.second;
    }
    else {
        //graph[edgePair.first].flow += 1;
        out << "existing edge- " << edgePair.first << " (E=" << dec << num_edges(graph) << ")" << endl; 
    }
    return edgePair.second;
}


bool CallGraph::addEdge(unsigned int a1, unsigned int a2) {
    // Get the nodes by address, then add an edge
    CallGraphNodeId n1 = getNode(a1);
    CallGraphNodeId n2 = getNode(a2);

    return addEdgeNodeId(n1, n2);
}
/*
void CallGraph::write(const std::string& fname) {
    //boost::dynamic_properties dp;
    //dp.property("id", get(name, graph));
    
    ofstream out(fname.c_str());
    write_graphviz(out, graph);
    
}
*/

CallGraph::~CallGraph() {
    out << "destruct the callgraph" << endl << flush;
    out << "############################################" << flush;
}

