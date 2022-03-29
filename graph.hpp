#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cmath>
#include <forward_list>
#include <limits>
#include <queue>
#include <unordered_set>
#include <vector>

#include "hash_map.hpp"

namespace ds {

namespace detail {

template <typename Map>
std::vector<typename Map::key_type> findKeysWithMinValue(const Map& map) {
    if (map.empty()) {
        return {};
    }

    auto i      = map.begin();
    auto minPos = i;

    std::vector<typename Map::key_type> res = {i->first};
    for (++i; i != map.end(); ++i) {
        if (i->second < minPos->second) {
            minPos = i;
            res    = {i->first};
        } else if (i->second == minPos->second) {
            res.push_back(i->first);
        }
    }
    return res;
}

}  // namespace detail

template <typename T, typename HashFunc = std::hash<T>>
class Graph {
public:
    Graph() : initialized_(false) {}

    void addEdge(const T& vertex1, const T& vertex2, int weight) {
        addEdgeImpl(vertex1, vertex2, weight);
        addEdgeImpl(vertex2, vertex1, weight);
    }

    std::unordered_set<T> vertices() const {
        std::unordered_set<T> res;
        auto keySelector = [](const std::pair<T, std::forward_list<std::pair<T, int>>>& pair) { return pair.first; };
        std::transform(adjacencyList_.begin(), adjacencyList_.end(), std::inserter(res, res.begin()), keySelector);
        return res;
    }

    HashMap<T, int> findShortestDistancesDijkstra(const T& srcVertex) const {
        HashMap<T, int> distances;
        HashMap<T, bool> visited;
        for (const auto& pair : adjacencyList_) {
            const auto& vertex = pair.first;
            visited[vertex]    = false;
            distances[vertex]  = INT32_MAX;
        }

        using DistanceVertex = std::pair<int, T>;
        std::priority_queue<DistanceVertex, std::vector<DistanceVertex>, std::greater<DistanceVertex>> heap;

        heap.emplace(0, srcVertex);
        distances[srcVertex] = 0;

        while (!heap.empty()) {
            auto v = heap.top().second;
            if (!visited[v]) {
                distances[v] = heap.top().first;
                visited[v]   = true;
            }
            heap.pop();

            for (const auto& pair : adjacencyList_.find(v)->second) {
                const auto& neighbor = pair.first;
                auto weight          = pair.second;

                if (visited[neighbor]) {
                    continue;
                }

                if (distances[neighbor] > weight + distances[v]) {
                    distances[neighbor] = weight + distances[v];
                    heap.emplace(distances[neighbor], neighbor);
                }
            }
        }
        return distances;
    }

    std::vector<T> findFairVertices(const std::vector<T>& sourceVertices) {
        if (!initialized_) {
            init();
            initialized_ = true;
        }
        HashMap<T, int> vertexToFairScore;
        for (const auto& pair : adjacencyList_) {
            const auto& vertex        = pair.first;
            const auto& distances     = dijkstraResults_.find(vertex)->second;
            vertexToFairScore[vertex] = 0;
            for (int i = 0; i < sourceVertices.size(); ++i) {
                for (int j = i + 1; j < sourceVertices.size(); ++j) {
                    auto a = distances.find(sourceVertices[i])->second;
                    auto b = distances.find(sourceVertices[j])->second;
                    vertexToFairScore[vertex] += std::abs(a - b);
                }
            }
        }
        return detail::findKeysWithMinValue(vertexToFairScore);
    }

private:
    HashMap<T, std::forward_list<std::pair<T, int>>, HashFunc> adjacencyList_;
    HashMap<T, HashMap<T, int, HashFunc>, HashFunc> dijkstraResults_;
    bool initialized_;

    void addEdgeImpl(const T& vertex1, const T& vertex2, int weight) {
        if (adjacencyList_.find(vertex1) == adjacencyList_.end()) {
            adjacencyList_[vertex1] = {
                {vertex2, weight}
            };
        } else {
            adjacencyList_[vertex1].emplace_front(vertex2, weight);
        }
    }

    void init() {
        for (const auto& pair : adjacencyList_) {
            const auto& vertex       = pair.first;
            dijkstraResults_[vertex] = findShortestDistancesDijkstra(vertex);
        }
    }
};

}  // namespace ds

#endif  // GRAPH_HPP
