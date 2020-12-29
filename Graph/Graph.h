    #pragma once

    #include <vector>
    #include <optional>
    #include <map>
    #include <queue>
    #include <iostream>
    #include <float.h>
    #include <bits/stdc++.h>

    // define 1 if you want to implement it
    #define BONUS 1

    // Edge can be constructed with syntax
    // Edge e{ 1, 5, 1.89 };
    struct Edge {
        int v;
        int u;
        double weight;
    };

    /* Graph represents one abstract graph.
     *
     * Vertices are always numbered from 0.
     *
     * Between two vertices,  there can only be one edge.   If edges is in
     * form of  (v, v), so  both ends are  the same, e.g.   AddEdge({1, 1,
     * 0.5}) throw an exception.
     *
     * Also throw exception if one or both vertices are out of bounds.
     *
     * Graph is not directed so (u, v) is the same edge as (v, u).
     *
     * Weights are  limited to positive  values, so 0 and  negative values
     * are forbidden.  If you encounter such weights throw an exception.
     */

    class Graph {
        // Do not modify public interface
    public:
        // Construct graph with n vertices and no edges
    //    explicit Graph(size_t n);
        explicit Graph(size_t n) {
            for (size_t i = 0; i < n; i++) {
                edge_table[i] = std::unordered_map<int, double>();
            }
        }

        /* Will construct graph with given  edges, the vertices are from 0
         * to whatever is the highest number in the vector of edges.
         */
        explicit Graph(const std::vector<Edge> &edges) {
            int max = -1;

            for (auto i = edges.cbegin(); i != edges.cend(); ++i) {
                if (i->u == i->v || i->weight < 0) {
                    throw std::exception();
                }

                max = max < i->u ? i->u : max;
                max = max < i->v ? i->v : max;

                edge_table[i->u][i->v] = i->weight;
                edge_table[i->v][i->u] = i->weight;
            }
            for (int i = 0; i < max; i++) {
                if (edge_table.find(i) == edge_table.end()) {
                    edge_table[i] = std::unordered_map<int, double>();
                }
            }
        }

        /* Add  edge to  graph. If  the edge  already exists,  replace the
         * weight.
         */

        void AddEdge(const Edge &edge) {
            if (edge.u == edge.v || edge.weight < 0 || edge_table.find(edge.u) == edge_table.end() ||
                edge_table.find(edge.v) == edge_table.end()) {

                throw std::exception();
            }
            edge_table[edge.u][edge.v] = edge.weight;
            edge_table[edge.v][edge.u] = edge.weight;
        }

        /* Same as AddEdge, but can insert  multiple edges. If one edge is
         * there more  than once,  use later one  (edge with  higher index
         * overwrites edge with lower index)
         */
        void AddEdges(const std::vector<Edge> &edges) {
            try {
                for (auto const &value: edges) {
                    this->AddEdge(value);
                }
            }
            catch (const char * e){
                throw e;
            }
        }

        /* Return  weight between  vertices  u  and v.  If  edge does  not
         * exists, behaviour is undefined.
         */
        double operator()(int u, int v) const {
            return edge_table.find(u)->second.find(v)->second;
        }

        /* Return weight between vertices u and v, if edge does not exists,
         * throw an exception.
         */
        double At(int u, int v) const {
            if (edge_table.find(u) != edge_table.end() &&
                edge_table.find(u)->second.find(v) != edge_table.find(u)->second.end()) {
                return edge_table.find(u)->second.find(v)->second;
            } else {
                throw std::exception();
            }
        }

        /* Return true  if there  is an  edge between  u and  v, otherwise
         * false.
         */
        bool Connected(int u, int v) const noexcept {
            try {
                this->At(u, v);
            }
            catch (const std::exception& e) {
                return false;
            }
            return true;
        }

        /* Return shortest path  from u to v (path with  minimal cost). If
         * there is no path return nullopt.   You can use dijkstra, or any
         * other algorithm. Path should start with u and ends with v.
         *
         * https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
         *
         * 1 BONUS point will be for  speed of this function.
         * First 5 implementations will be awarded with the bonus point.
         */
        std::optional<std::vector<int>> Path(int u, int v) const {
            std::priority_queue<
                    std::pair<double, int>,
                    std::vector<std::pair<double, int>>,
                    std::greater<std::pair<double, int>>> pq;
            std::unordered_map<int, double> distance_table;
            std::unordered_map<int, int> parent_table;

            for (auto const&[key, val] : edge_table) {
                distance_table[key] = DBL_MAX;
            }

            if (u == v) {
                return std::nullopt;
            }

            if (edge_table.find(u) != edge_table.end() && edge_table.find(v) != edge_table.end()) {
                pq.push(std::make_pair(0, u));
                distance_table[u] = 0;
                parent_table[u] = -1;

                while (!pq.empty()) {
                    auto[cost, index] = pq.top();
                    pq.pop();
                    if (index == v) {
                        std::vector<int> result;
                        int key = v;
                        while (true) {
                            result.push_back(key);
                            key = parent_table[key];
                            if (key == -1) {
                                std::reverse(result.begin(), result.end());
                                return result;
                            }
                        }
                    } else {
                        for (auto const&[key, value] : edge_table.find(index)->second) {
                            if (distance_table[key] > value + cost) {
                                distance_table[key] = value + cost;
                                parent_table[key] = index;
                                pq.push(std::make_pair(value + cost, key));
                            }
                        }
                    }
                }
            }
            return std::nullopt;
        }

    #if BONUS == 1
        /* Returns  minimum spanning  tree  for this  graph.  You can  use
         * kruskal's algorithm
         * https://en.wikipedia.org/wiki/Kruskal%27s_algorithm
         */
        Graph SpannigTree() const {
            Graph spanning_tree_graph = Graph(edge_table.size());
            std::vector<std::set<int>> set_vector;
            unsigned long set_index1, set_index2;
            std::priority_queue<
                     std::pair<double, std::pair<int, int>>,
                     std::vector<std::pair<double, std::pair<int, int>>>,
                     std::greater<std::pair<double, std::pair<int, int>>>
                    > pq;
            for (auto const&[key1, nested_map] : edge_table) {
                for (auto const&[key2, cost] : edge_table.find(key1)->second){
                    auto node = std::make_pair(cost, std::make_pair(key1,key2));
                    pq.push(node);
                }
            }

            for (unsigned long i = 0; i < edge_table.size(); ++i) {
                std::set<int> tmp = {};
                tmp.insert(static_cast<int>(i));
                set_vector.push_back(tmp);
            }

            while (!pq.empty()){
                auto[cost, index] = pq.top();
                pq.pop();
                for (unsigned long i = 0; i < set_vector.size(); i++){
                    if (set_vector[i].find(index.first) != set_vector[i].end()){
                        set_index1 = i;
                    }
                    if (set_vector[i].find(index.second) != set_vector[i].end()){
                        set_index2 = i;
                    }
                }
                if (set_index1 != set_index2){
                    Edge edge = {index.first, index.second, cost};
                    spanning_tree_graph.AddEdge(edge);
                    set_vector[set_index1].insert(set_vector[set_index2].begin(), set_vector[set_index2].end());
                    set_vector.erase(set_vector.begin()+ static_cast<long>(set_index2));
                }
            }
            return spanning_tree_graph;
         }
    #endif

    private:
        std::unordered_map<int, std::unordered_map<int, double>> edge_table;
    };