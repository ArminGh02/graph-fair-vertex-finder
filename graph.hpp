#include <cmath>
#include <forward_list>
#include <limits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// #define FMT_HEADER_ONLY
// #include <fmt/color.h>
// #include <fmt/core.h>

namespace ds {

namespace detail {

template <typename Map>
auto findKeysWithMinValue(const Map& map) -> std::vector<typename std::remove_const<decltype(std::begin(map)->first)>::type> {
    using K = typename std::remove_const<decltype(std::begin(map)->first)>::type;
    if (map.empty()) {
        return {};
    }
    auto i = map.begin();
    auto minIter = i;
    std::vector<K> res = {i->first};
    for (++i; i != map.end(); ++i) {
        if (i->second < minIter->second) {
            minIter = i;
            res = {i->first};
        } else if (i->second == minIter->second) {
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
        auto keySelector = [](const std::pair<T, std::forward_list<std::pair<T, int>>>& pair) {
            return pair.first;
        };
        std::transform(adjacencyList_.begin(), adjacencyList_.end(), std::inserter(res, res.begin()), keySelector);
        return res;
    }

    std::unordered_map<T, int> findShortestDistancesDijkstra(const T& srcVertex) const {
        std::unordered_map<T, int> distances;
        std::unordered_map<T, bool> visited;
        for (const auto& pair : adjacencyList_) {
            const auto& vertex = pair.first;
            visited[vertex] = false;
            distances[vertex] = INT32_MAX;
        }

        using DistanceVertex = std::pair<int, T>;
        std::priority_queue<DistanceVertex, std::vector<DistanceVertex>, std::greater<DistanceVertex>> heap;

        heap.emplace(0, srcVertex);
        distances[srcVertex] = 0;

        while (!heap.empty()) {
            auto v = heap.top().second;
            if (!visited[v]) {
                distances[v] = heap.top().first;
                visited[v] = true;
            }
            heap.pop();

            for (const auto& pair : adjacencyList_.find(v)->second) {
                const auto& neighbor = pair.first;
                auto weight = pair.second;

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

        if (sourceVertices.size() == 1) {
            return {sourceVertices[0]};
        }

        // for (const auto& pair : dijkstraResults_) {
        //     fmt::print("dijkstra for {}:\n", pair.first);
        //     for (const auto& pair2 : pair.second) {
        //         fmt::print("distance from {} is {}\n", pair2.first, pair2.second);
        //     }
        //     fmt::print("------------------------------------\n");
        // }

        std::unordered_map<T, int> vertexToFairScore;
        for (const auto& pair : adjacencyList_) {
            const auto& vertex = pair.first;
            // fmt::print("vertex: {}\n", vertex);
            const auto& distances = dijkstraResults_.find(vertex)->second;
            vertexToFairScore[vertex] = 0;
            for (int i = 0; i < sourceVertices.size(); ++i) {
                for (int j = i + 1; j < sourceVertices.size(); ++j) {
                    auto a = distances.find(sourceVertices[i])->second;
                    auto b = distances.find(sourceVertices[j])->second;
                    // fmt::print("a: {}, b: {}\n", a, b);
                    vertexToFairScore[vertex] += std::abs(a - b);
                }
            }
        }

        // for (const auto& pair : vertexToFairScore) {
        //     fmt::print("fair score of {} is {}", pair.first, pair.second);
        // }

        return detail::findKeysWithMinValue(vertexToFairScore);
    }

   private:
    std::unordered_map<T, std::forward_list<std::pair<T, int>>, HashFunc> adjacencyList_;
    std::unordered_map<T, std::unordered_map<T, int>> dijkstraResults_;
    bool initialized_;

    void addEdgeImpl(const T& vertex1, const T& vertex2, int weight) {
        if (adjacencyList_.find(vertex1) == adjacencyList_.end()) {
            adjacencyList_[vertex1] = {{vertex2, weight}};
        } else {
            adjacencyList_[vertex1].emplace_front(vertex2, weight);
        }
    }

    void init() {
        for (const auto& pair : adjacencyList_) {
            const auto& vertex = pair.first;
            dijkstraResults_[vertex] = findShortestDistancesDijkstra(vertex);
        }
    }
};

}  // namespace graph
