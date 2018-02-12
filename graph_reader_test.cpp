//
// Created by saber on 2/9/2018.
//
#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>

using namespace lemon;
typedef ListGraph Graph;
typedef Graph::Node Node;
typedef Graph::Arc Arc;
using NodeIt = Graph::NodeIt;
using ArcIt = Graph::ArcIt;
using OutArcIt = Graph::OutArcIt;
template <>
std::vector<const Node *> getRoots(const Graph *g){
    std::vector<const Node *> roots;
    for (NodeIt i(*g);i!=INVALID;++i){
        OutArcIt out(*g, i);
        if (out == INVALID) roots.push_back(&i);
    }
    return roots;
}

int main()
{
<<<<<<< HEAD

=======
    typedef ListDigraph Graph;
    typedef Graph::Node Node;
    typedef Graph::Arc Arc;
    using NodeIt = Graph::NodeIt;
    using ArcIt = Graph::ArcIt;
>>>>>>> 51054337c0226048e7ab8cd56da1ec03a5014ae8
    Graph g;
    std::string filename("example.lfg");
    try{
        DigraphReader<Graph> reader(g, filename);
        reader.run();
    }
    catch (Exception& error) { // check if there was any error
        std::cerr << "Error: " << error.what() << std::endl;
        return -1;
    }


    std::cout << "Nodes:";
    for (NodeIt i(g); i!=INVALID; ++i)
        std::cout << " " << g.id(i);
    std::cout << std::endl;

    std::cout << "Edges:";
    for (ArcIt i(g); i!=INVALID; ++i)
        std::cout << " (" << g.id(g.source(i)) << "," << g.id(g.target(i)) << ")";
    std::cout << std::endl;
    std::cout <<  std::endl;

    return 0;
}
