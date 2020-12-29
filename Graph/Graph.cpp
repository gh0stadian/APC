//
// Created by gh0stadian on 12/4/20.
//

#include "Graph.h"
#include <assert.h>

int main() {
    std::vector<Edge> edges;
    for (int i = 1; i < 12; i++) {
        Edge edge = {i - 1, i, 20 + (i * 0.5) + (i - 2)};
        Edge edge2 = {i, (i + 1 + i % 4), 12 + (i * 0.7) + (i - 2)};
        if (i % 2 == 1) {
            Edge edge3 = {i, (i / 2), 21 + (i * 0.1)};
            edges.push_back(edge3);
        }
        edges.push_back(edge);
        edges.push_back(edge2);
    }
    Graph graph = Graph(edges);

    assert (graph.Connected(1, 3) != false);
    assert (graph.At(1, 3) == 21.3);
    Graph graph2 = Graph(5);
    assert (graph2.Connected(1, 3) == false);
    graph2.AddEdge(Edge{1, 3, 2.0});
    try {
        graph2.AddEdge(Edge{1, 6, 2.0});
    }
    catch (const std::exception& e) {
        std::cout << "OK Edge{1,6,2.0}\n";
    }
    try {
        graph2.AddEdge(Edge{1, 1, 2.0});
    }
    catch (const std::exception& e) {
        std::cout << "OK Edge{1,1,2.0}\n";
    }
    try {
        graph2.AddEdge(Edge{1, 2, -2.0});
    }
    catch (const std::exception& e) {
        std::cout << "OK Edge{1,2,-2.0}\n";
    }
    assert(graph.Path(0, 0) == std::nullopt);
    assert(graph.Path(0, 12) == std::nullopt);
    graph.AddEdge(Edge{0, 12, 1.0});
    std::vector<int> result;
    result = {0, 1, 3, 4};
    assert(graph.Path(0, 4) == result);
    result = {0, 1, 2, 5, 11, 15};
    assert(graph.Path(0, 15) == result);
    result = {0, 12};
    assert(graph.Path(0, 12) == result);
}

//    assert();
//    try {
//        std::cout << graph.At(1,3) << std::endl;
//    }
//    catch (const char * e){
//        std::cout << e << std::endl;
//    }
//    std::cout << graph.Connected(1,2) << std::endl;

//    graph.printer();
//    std::optional<std::vector<int>> result = graph.Path(0,15);
//    if (result != std::nullopt){
//        for (auto const&i : *result){
//            std::cout << i << " ";
//        }
//        std::cout << std::endl;
//    }
//    std::cout << std::endl;
//    Graph st = graph.SpannigTree();
//    st.printer();
//}