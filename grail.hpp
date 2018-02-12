/*
 * Filename : grail.hpp
 *
 * Description :
 *     This file implements the reachability query algorithm proposed in
 *     "GRAIL: Scalable Reachability Index for Large Graphs".
 *
 * Note:
 *     To use the this Grail class, you need to define two functions for your graph type.
 *     The two functions are:
 *         1. std::vector<const Node *> getRoots(const Graph *g) and,
 *         2. std::vector<const Node *> getChildren(const Graph *g, const Node *u).
 *     Here, Graph and Node are the types for you graph and nodes respectively.
 */
#include <algorithm>    // std::random_shuffle
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector
#include <lemon/list_graph.h>
#include <lemon/lgf_reader.h>

using namespace lemon;


namespace saber {
    using Interval=std::pair<size_t, size_t>;
    typedef ListDigraph Graph;
    typedef Graph::Node Node;
    using NodeIt = Graph::NodeIt;
    using ArcIt = Graph::ArcIt;
    using OutArcIt = Graph::OutArcIt;
    using InArcIt = Graph::InArcIt;

    bool contains(const Interval &B, const Interval &A) {
        return (A.first >= B.first && A.second <= B.second);
    }

    class Grail {
    public:

        explicit Grail(Graph &g) : _g(&g), _dim(5), _rank(1), _labels(_dim) {

            for (NodeIt ui(*_g);ui!=INVALID;++ui) {
                size_t uid = _g->id(ui);
                for (size_t i = 0; i < _dim; ++i) {
                    _labels[i].insert({uid, std::make_pair(0, 0)});
                }
                InArcIt  in(*_g, ui);
                if (in == INVALID) {
                    _roots.push_back(uid);
                }
            }
            std::cout << "roots : \n";
            for (auto v: _roots){
                std::cout << v << " ";
            }
            std::cout << std::endl;

        }

        void buildIndex(){
            randomizedLabeling();

            for (auto l: _labels[0])
                std::cout << l.first
                          << " : ["
                          << (l.second).first
                          << ", "
                          << (l.second).second
                          << "]\n";
        }

        bool reachable(size_t u, size_t v) {
            for (size_t i = 0; i < _dim; ++i) {
                if (!contains(_labels[i][u], _labels[i][v])) {
                    // std::cout << "!!!hit!!!\n";
                    return false;
                }
            }
            std::vector<size_t > children;
            for (OutArcIt a(*_g, _g->nodeFromId(u));a!=INVALID;++a) {
                const Node nu = _g->target(a);
                children.push_back(_g->id(nu));
            }
            for (auto c: children) {
                if (c == v || reachable(c, v)) return true;
            }
            return false;
        }

    private:
        Graph *_g;
        size_t _dim;
        size_t _rank;
        std::unordered_map<size_t, bool> _visited;
        std::vector<std::unordered_map<size_t, Interval> > _labels;
        std::vector<size_t> _roots;

        void randomizedLabeling() {
            for (size_t i = 0; i < _dim; ++i) {
                _rank = 1;
                for (NodeIt ni(*_g);ni!=INVALID;++ni){
                    _visited[_g->id(ni)] = false;
                }

                std::vector<size_t > roots(_roots.size());
                std::copy(_roots.begin(),_roots.end(),roots.begin());
                std::random_shuffle(roots.begin(), roots.end());
                for (auto x: roots) {
                    randomizedVisit(x, i);
                }
            }

        }

        void randomizedVisit(size_t x, size_t i) {
            if (_visited[x]) return;
            _visited[x] = true;
            std::vector<size_t > children;
            for (OutArcIt a(*_g, _g->nodeFromId(x));a!=INVALID;++a) {
                const Node v = _g->target(a);
                children.push_back(_g->id(v));
            }
            size_t minRankOfChildren = std::numeric_limits<size_t>::max();
            if (!children.empty()) {
                std::random_shuffle(children.begin(), children.end());
                for (auto y: children)
                    randomizedVisit(y, i);

                minRankOfChildren = _labels[i][children[0]].first;
                for (size_t k = 1; k < children.size(); ++k)
                    if (_labels[i][children[k]].first < minRankOfChildren)
                        minRankOfChildren = _labels[i][children[k]].first;
            }

            _labels[i][x].first = std::min(_rank, minRankOfChildren);
            _labels[i][x].second = _rank;
            ++ _rank;
        }
    };
}