#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "graph.hpp"

#define FMT_HEADER_ONLY
#include <fmt/color.h>
#include <fmt/core.h>

void printPrompt();
void printHeader();
void getGraph(ds::Graph<int>& g);
void join(std::vector<int>& srcVertices);
void leave(std::vector<int>& srcVertices);
template <typename Container>
void printContainer(const Container& c);

int main() {
    printHeader();
    fmt::print("\n\nEnter the desired graph:\n");
    printPrompt();

    ds::Graph<int> g;
    getGraph(g);

    fmt::print(
        fmt::fg(fmt::color::green),
        "Available commands:\n"
        "join <vertex number>\n"
        "left <vertex number>\n"
        "test\t\t// prints all the vertices of the given graph\n"
        "calc\t\t// calculates the fair vertices\n"
        "exit\t\t// exits the program\n");

    std::string command;
    std::vector<int> srcVertices;
    while (true) {
        printPrompt();
        std::cin >> command;
        if (command == "join") {
            join(srcVertices);
        } else if (command == "left") {
            leave(srcVertices);
        } else if (command == "test") {
            printContainer(g.vertices());
            fmt::print("\n");
        } else if (command == "calc") {
            fmt::print("sources: ");
            printContainer(srcVertices);
            fmt::print("\n");
            fmt::print(fmt::fg(fmt::color::green), "The fairest vertices are:\n");
            printContainer(g.findFairVertices(srcVertices));
            fmt::print("\n");
        } else if (command == "exit") {
            break;
        }
    }

    return 0;
}

void printPrompt() {
    fmt::print(fmt::fg(fmt::color::green), "$");
    fmt::print(" ");
}

void printHeader() {
    const auto style = fmt::fg(fmt::color::green) | fmt::emphasis::reverse;
    fmt::print(style, "This is the final project of DS course by ");
    fmt::print(style | fmt::emphasis::bold, "Armin Gh");
}

void getGraph(ds::Graph<int>& g) {
    int nodesCount;
    int edgesCount;
    std::cin >> nodesCount >> edgesCount >> std::ws;

    std::string line;
    std::getline(std::cin, line);

    for (int i = 0; i < edgesCount; ++i) {
        int u;
        int v;
        int weight;
        std::cin >> u >> v >> weight;
        g.addEdge(u, v, weight);
    }
}

void join(std::vector<int>& srcVertices) {
    int vertex;
    std::cin >> vertex;
    srcVertices.push_back(vertex);
    fmt::print(fmt::fg(fmt::color::green), "Joined successfully.\n");
}

void leave(std::vector<int>& srcVertices) {
    int vertex;
    std::cin >> vertex;
    auto it = std::find(srcVertices.begin(), srcVertices.end(), vertex);
    if (it == srcVertices.end()) {
        fmt::print(fmt::fg(fmt::color::red), "The person was not joined.\n");
    } else {
        srcVertices.erase(it);
        fmt::print(fmt::fg(fmt::color::green), "Executed successfully.\n");
    }
}

template <typename Container>
void printContainer(const Container& c) {
    fmt::print("[");
    if (!c.empty()) {
        for (auto i = c.begin();;) {
            fmt::print("{}", *i);
            if (++i == c.end()) break;
            fmt::print(", ");
        }
    }
    fmt::print("]");
}
