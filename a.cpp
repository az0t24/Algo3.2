#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <limits>

class Graph {
public:
    virtual void insert_edge(const int32_t& first, const int32_t& second) = 0;

    virtual int32_t finding_shortest_path_with_saving(std::vector<int32_t>& answer,
                                                      const int32_t& start,
                                                      const int32_t& end) = 0;
};

class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum, size_t edgesNum) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        edges_.resize(vertexesNum_ + 1, {});
    }

    void insert_edge(const int32_t& first, const int32_t& second) {
        edges_[first].push_back(second);
        edges_[second].push_back(first);
    }

    int32_t finding_shortest_path_with_saving(std::vector<int32_t>& answer,
                                          const int32_t& start,
                                          const int32_t& end) {
        return finding_shortest_path_with_saving_private(answer, start, end);
    }
    
private:
    size_t vertexesNum_;
    size_t edgesNum_;
    std::vector<std::vector<int32_t>> edges_; // using adjacency list

    int32_t finding_shortest_path_with_saving_private(std::vector<int32_t>& answer,
                                                      const int32_t& start,
                                                      const int32_t& end) {
        std::vector<size_t> dist(vertexesNum_ + 1, vertexesNum_ + 1);
        std::vector<size_t> prev(vertexesNum_ + 1, 0);
        std::queue<int32_t> tempQueue;
        tempQueue.push(start);
        dist[start] = 0;
        int32_t elem_v = 0;

        while (!tempQueue.empty()) {
            elem_v = tempQueue.front();
            tempQueue.pop();
            for (int32_t elem_u : edges_[elem_v]) {
                if (dist[elem_u] == vertexesNum_ + 1) {
                    dist[elem_u] = dist[elem_v] + 1;
                    prev[elem_u] = elem_v;
                    tempQueue.push(elem_u);
                }
            }
        }
        if (dist[end] == vertexesNum_ + 1) {
            return -1;
        }
        
        int32_t lenth = dist[end];
        int32_t last = prev[end];
        answer.push_back(end);
        for (int32_t i = 0; i < lenth; ++i) {
            answer.push_back(last);
            last = prev[last];
        }

        return lenth;
    }
};

int main() {
    int32_t n = 0;
    int32_t m = 0;
    std::cin >> n >> m;

    int32_t a = 0;
    int32_t b = 0;
    std::cin >> a >> b;

    GraphList graph(n, m);
    int32_t first = 0;
    int32_t second = 0;
    for (int32_t i = 0; i < m; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::vector<int32_t> answer = {};
    int32_t lenth = graph.finding_shortest_path_with_saving(answer, a, b);

    std::cout << lenth << std::endl;
    for (int32_t i = 0; i < lenth + 1; ++i) {
        std::cout << answer.back() << " ";
        answer.pop_back();
    }
    
    return 0;
}