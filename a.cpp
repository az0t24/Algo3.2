#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

typedef int64_t dist_t;
typedef int64_t vertex_t;
typedef int64_t weight_t;

class Graph {
public:
    virtual void insert_edge(const vertex_t& first, const vertex_t& second,
                             const weight_t& weight = 0) = 0;

    virtual dist_t finding_shortest_path_with_saving(std::vector<vertex_t>& answer,
                                                      const vertex_t& start,
                                                      const vertex_t& end) = 0;
};

// GraphList uses adjacency list for edges
class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum) {
        vertexesNum_ = vertexesNum;
        edges_.resize(vertexesNum_, {});
        kMaxValue_ = vertexesNum + 1;
    }

    void insert_edge(const vertex_t& first, const vertex_t& second,
                                            const weight_t& weight = 0) {
        edges_[first - 1].push_back(second - 1);
        edges_[second - 1].push_back(first - 1);
    }

    dist_t finding_shortest_path_with_saving(std::vector<vertex_t>& answer,
                                             const vertex_t& start,
                                             const vertex_t& end) {
        std::vector<dist_t> dist(vertexesNum_, kMaxValue_);
        std::vector<vertex_t> prev(vertexesNum_, 0);
        std::queue<vertex_t> tempQueue;

        tempQueue.push(start - 1);
        dist[start - 1] = 0;

        vertex_t currentVertex = 0;

        while (!tempQueue.empty()) {
            currentVertex = tempQueue.front();
            tempQueue.pop();

            for (int32_t currentNeighbour : edges_[currentVertex]) {
                if (dist[currentNeighbour] == kMaxValue_) {
                    dist[currentNeighbour] = dist[currentVertex] + 1;
                    prev[currentNeighbour] = currentVertex;

                    tempQueue.push(currentNeighbour);
                }
            }
        }
        if (dist[end - 1] == kMaxValue_) {
            return kErrorCode_;
        }
        
        dist_t length = dist[end - 1];
        
        normalize_path(prev, answer, length, end - 1);

        return length;
    }
    
private:
    size_t vertexesNum_;
    std::vector<std::vector<vertex_t>> edges_; // using adjacency list
    size_t kMaxValue_;
    const int8_t kErrorCode_ = -1;

    void normalize_path(const std::vector<vertex_t>& prev, std::vector<vertex_t>& answer, const dist_t& length, const vertex_t& end) {
        vertex_t current = prev[end];
        answer.push_back(end + 1);

        for (int32_t i = 0; i < length; ++i) {
            answer.push_back(current + 1);
            current = prev[current];
        }
    }
};

int main() {
    int32_t vertexNum = 0;
    int32_t edgesNum = 0;
    std::cin >> vertexNum >> edgesNum;

    vertex_t start = 0;
    vertex_t end = 0;
    std::cin >> start >> end;

    GraphList graph(vertexNum);
    vertex_t first = 0;
    vertex_t second = 0;
    for (size_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::vector<vertex_t> answer = {};
    dist_t length = graph.finding_shortest_path_with_saving(answer, start, end);

    std::cout << length << std::endl;
    for (int32_t i = 0; i < length + 1; ++i) {
        std::cout << answer.back() << " ";
        answer.pop_back();
    }
    
    return 0;
}