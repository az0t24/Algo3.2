#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
#include <queue>

typedef int32_t dist_t;
typedef int32_t vertex_t;
typedef int32_t weight_t;

class Graph {
public:
    struct GraphNeighboursNode {
        vertex_t vertex_;
        weight_t weight_;
    };

    struct Edge {
        vertex_t from_;
        vertex_t to_;
        weight_t weight_;
        int32_t number_;

        friend bool operator<(const Edge& first, const Edge& second) {
            return first.from_ < second.from_ || (first.from_ == second.from_ && first.to_ <= second.to_);
        }

        friend bool operator==(const Edge& first, const Edge& second) {
            return first.from_ == second.from_ && first.to_ == second.to_ && first.weight_ == second.weight_;
        }
    };

    typedef enum {
        WHITE_COLOUR,
        GREY_COLOUR,
        BLACK_COLOUR,
    } vertex_colours_t;

    size_t vertexesNum_;
    size_t edgesNum_;
    bool isOriented_ = false;

    static const int32_t UNDEFINED;

    virtual std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) const = 0;
    virtual std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) const = 0;
    virtual std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) const = 0;
    virtual void insert_edge(const vertex_t& x, const vertex_t& y, const weight_t& weight = 0) = 0;
};

class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum, size_t edgesNum = 0) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        adjacencyList_.resize(vertexesNum_ + 1, {});
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        if (isOriented_) {
            insert_edge_oriented(first, second, weight);
        } else {
            insert_edge_not_oriented(first, second, weight);
        }
    }

    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) const {
        std::vector<vertex_t> answer(adjacencyList_[vert].size());
        for (size_t i = 0; i < answer.size(); ++i) {
            answer[i] = adjacencyList_[vert][i].to_;
        }
        return answer;
    }

    std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) const {
        std::vector<weight_t> answer(adjacencyList_[vert].size());
        for (size_t i = 0; i < answer.size(); ++i) {
            answer[i] = adjacencyList_[vert][i].weight_;
        }
        return answer;
    }

    std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) const {
        std::vector<GraphNeighboursNode> answer(adjacencyList_[vert].size());
        for (size_t i = 0; i < answer.size(); ++i) {
            answer[i].vertex_ = adjacencyList_[vert][i].to_;
            answer[i].weight_ = adjacencyList_[vert][i].weight_;
        }
        return answer;
    }

private:
    std::vector<std::vector<Edge>> adjacencyList_; // using adjacency list

    void insert_edge_oriented(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyList_[first].push_back({first, second, weight});
    }

    void insert_edge_not_oriented(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyList_[first].push_back({first, second, weight});
        adjacencyList_[second].push_back({second, first, weight});
    }
};

const int32_t Graph::UNDEFINED = INT_MAX;

std::vector<vertex_t> normalize_path(const std::vector<vertex_t>& prev, const dist_t& length, const vertex_t& end) {
    std::vector<vertex_t> answer;
    vertex_t current = prev[end];
    answer.push_back(end);

    for (int32_t i = 0; i < length; ++i) {
        answer.push_back(current);
        current = prev[current];
    }

    std::reverse(answer.begin(), answer.end());
    return answer;
}

std::vector<vertex_t> find_shortest_path_with_saving(const Graph& graph,
                                                      const vertex_t& start,
                                                      const vertex_t& end) {
    std::vector<dist_t> dist(graph.vertexesNum_ + 1, Graph::UNDEFINED);
    std::vector<vertex_t> prev(graph.vertexesNum_ + 1, 0);
    std::queue<vertex_t> tempQueue;

    tempQueue.push(start);
    dist[start] = 0;

    vertex_t currentVertex = 0;

    while (!tempQueue.empty()) {
        currentVertex = tempQueue.front();
        tempQueue.pop();

        for (vertex_t currentNeighbour : graph.get_neighbours_vertex(currentVertex)) {
            if (dist[currentNeighbour] == Graph::UNDEFINED) {
                dist[currentNeighbour] = dist[currentVertex] + 1;
                prev[currentNeighbour] = currentVertex;

                tempQueue.push(currentNeighbour);
            }
        }
    }

    dist_t length = dist[end];
    if (length == Graph::UNDEFINED) {
        return {};
    }

    return normalize_path(prev, length, end);
}

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

    std::vector<vertex_t> answer = find_shortest_path_with_saving(graph, start, end);

    std::cout << (int)answer.size() - 1 << std::endl;
    for (auto elem : answer) {
        std::cout << elem << " ";
    }
    
    return 0;
}