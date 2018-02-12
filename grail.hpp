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

template <typename Graph, typename Node>
std::vector<const Node *> getRoots(const Graph *g);
template <typename Graph, typename Node>
std::vector<const Node *> getChildren(const Graph *, const Node *u);

namespace saber {
    using Interval=std::pair<size_t, size_t>;

    bool contains(const Interval &A, const Interval &B) const {
        return (A.first >= B.first && A.second <= B.second);
    }

    template<typename Graph, typename Node>
    class Grail {
    public:

        explicit Grail(const Graph &g) : _g(&g), _dim(5), _rank(1), _labels(_dim) {
            for (size_t i = 0; i < _dim; ++i) {
                for (Graph::iterator v = _g->begin(); v != _g->end(); ++v) {
                    const Graph::Node *pv = &(*v);
                    _labels[i][pv] = std::make_pair(0, 0);
                }
            }
        }

        void randomizedLabeling() {
            for (size_t i = 0; i < _dim; ++i) {
                _rank = 1;
                std::fill(_visited.begin(), _visited.end(), false);
                auto roots = getRoots<Graph, Node>(_g);
                std::random_shuffle(roots.begin(), roots.end());
                for (auto x: roots) {
                    randomizedVisit(x, i);
                }
            }

        }

        void randomizedVisit(const Node *x, size_t i) {
            if (_visited[x]) return;
            _visited[x] = true;
            auto children = getChildren<Graph, Node>(_g, x);
            std::random_shuffle(children.begin(), children.end());
            for (auto y: children)
                randomizedVisit(y, i);

            size_t minRankOfChildren = _labels[i][children[0]].first;
            for (size_t k = 1; k < children.size(); ++k)
                if (_labels[i][children[k]].first < minRankOfChildren)
                    minRankOfChildren = _labels[i][children[k]].first;

            _labels[i][x].first = std::min(_rank, minRankOfChildren);
            _labels[i][x].second = _rank;
            ++ _rank;
        }

        bool reachable(const Node *u, const Node *v) {
            for (size_t i = 0; i < _dim; ++i) {
                if (!contains(_labels[i][u], _labels[i][v]))
                    return false;
            }
            for (auto c: getChildren<Graph, Node>(_g, u)) {
                if (c == v || reachable(c, v)) return true;
            }
            return false;
        }

    private:
        Graph *_g;
        size_t _dim;
        size_t _rank;
        std::unordered_map<const Node *, bool> _visited;
        std::vector<std::unordered_map<const Node *, Interval> > _labels;
    };
}