//
// Created by saber on 2/9/2018.
//
#include <iostream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>

using namespace lemon;


int main()
{

    typedef ListDigraph Graph;
    typedef Graph::Node Node;
    typedef Graph::Arc Arc;
    using NodeIt = Graph::NodeIt;
    using ArcIt = Graph::ArcIt;
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
