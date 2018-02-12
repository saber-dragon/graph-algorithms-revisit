#include <iostream>
#include <fstream>
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>
#include <lemon/dfs.h>
#include "grail.hpp"
#include <chrono>
#include <random>

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

    size_t n = countNodes(g);
    size_t m = countArcs(g);

    std::cout << "# of nodes: "
              << n
              << ", # of edges: "
              << m
              << std::endl;
    saber::Grail grail(g);
    grail.buildIndex();
    Dfs<Graph> dfs(g);


    // correctness test
    size_t NUM_TEST = size_t(1e3);
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<size_t> dis(0, n - 1);

    for (size_t i = 0;i < NUM_TEST;++ i) {
        size_t u =  dis(gen);
        size_t v = dis(gen);
        while (v == u) v = dis(gen);
        auto f1 = grail.reachable(u, v);
        Node nv = g.nodeFromId(v);
        dfs.run(g.nodeFromId(u), nv);
        auto f2 = dfs.reached(nv);
        if (f1 != f2)
            std::cerr << "Error occurs on reachability from "
                      << u
                      << " to "
                      << v
                      << " :\n"
                      << "Expected: "
                      << (f2?"reachable":"unreachable")
                      << ", Obtained: "
                      << (f1?"reachable":"unreachable")
                      << std::endl;

    }

    // efficiency test
    NUM_TEST = size_t(1e6);
    auto start = std::chrono::steady_clock::now();
    for (size_t t = 0;t < NUM_TEST;++ t){
        size_t u =  dis(gen);
        size_t v = dis(gen);
        while (v == u) v = dis(gen);
        grail.reachable(u, v);
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time elapsed : " << (double) duration.count() / NUM_TEST << std::endl;


    start = std::chrono::steady_clock::now();
    for (size_t t = 0;t < NUM_TEST;++ t){
        size_t u =  dis(gen);
        size_t v = dis(gen);
        while (v == u) v = dis(gen);
        Node nv = g.nodeFromId(v);
        dfs.run(g.nodeFromId(u), nv);
        dfs.reached(nv);
    }
    end = std::chrono::steady_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time elapsed : " << (double) duration.count() / NUM_TEST << std::endl;

//    for (NodeIt ui(g);ui!=INVALID;++ui){
//
//        for (NodeIt vi(g);vi!=INVALID;++vi){
//
//            if (ui != vi) {
//                dfs.run(ui, vi);
//                bool expected = dfs.reached(vi);
//                bool obtained = grail.reachable(g.id(ui), g.id(vi));
//                if (obtained != expected ){
//                    std::cout << "Error "
//                              << g.id(ui)
//                              << " --> "
//                              << g.id(vi)
//                              << " : "
//                              << "Expected " << expected
//                              << ", but obtained " << obtained << std::endl;
//                }
//            }
//        }
//    }

    return 0;
}

