#include <iostream>
#include <fstream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include "grail.hpp"

using namespace lemon;

typedef ListDigraph Graph;
typedef Graph::Node Node;
typedef Graph::Arc Arc;
using NodeIt = Graph::NodeIt;
using ArcIt = Graph::ArcIt;
using OutArcIt = Graph::OutArcIt;


int main()
{

    Graph g;
    std::string filename("dag.lgf");
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

    saber::Grail grail(g);
    grail.buildIndex();

    std::cout << "Index built" << std::endl;


    std::cout << "Edges:";
    for (ArcIt i(g); i!=INVALID; ++i)
        std::cout << " (" << g.id(g.source(i)) << "," << g.id(g.target(i)) << ")";
    std::cout << std::endl;
    std::cout <<  std::endl;

    for (NodeIt ui(g);ui!=INVALID;++ui){

        for (NodeIt vi(g);vi!=INVALID;++vi){

            if (ui != vi) {
                std::cout << g.id(ui)
                          << " --> "
                          << g.id(vi)
                          << " : "
                          << grail.reachable(g.id(ui), g.id(vi))
                          << std::endl;
            }
        }
    }

    return 0;
}

