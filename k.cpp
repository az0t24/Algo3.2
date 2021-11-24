#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>
#include <set>

typedef int32_t dist_t;
typedef int32_t vertex_t;
typedef int32_t weight_t;

struct Point{
    int32_t x;
    int32_t y;

    bool operator==(const Point& second) {
        return x == second.x && y == second.y;
    }
};

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
    
    size_t get_vertexes_num() const {
        return vertexesNum_;
    }

    size_t get_edges_num() const {
        return edgesNum_;
    }

    static const int32_t UNDEFINED;

    virtual std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) const = 0;
    virtual std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) const = 0;
    virtual std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) const = 0;
    virtual void insert_edge(const vertex_t& x, const vertex_t& y, const weight_t& weight = 0) = 0;

protected:
    size_t vertexesNum_;
    size_t edgesNum_;
    bool isOriented_ = false;
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

void fill_cycle(const vertex_t& start, const vertex_t& end,
                const std::vector<vertex_t>& prev, std::vector<vertex_t>& answer) {
    vertex_t currentVertex = end;

    while (currentVertex != start) {
        answer.push_back(currentVertex);
        currentVertex = prev[currentVertex];
    }
    
    answer.push_back(currentVertex);
    std::reverse(answer.begin(), answer.end());
}

bool check_component_for_cycle(const Graph& graph, const vertex_t& currentVertex, std::vector<vertex_t>& answer,
                               std::vector<bool>& used, std::vector<vertex_t>& prev) {
    used[currentVertex] = Graph::GREY_COLOUR;
    
    for (auto neighbour : graph.get_neighbours_vertex(currentVertex)) {
        if (used[neighbour] == Graph::GREY_COLOUR) {
            fill_cycle(neighbour, currentVertex, prev, answer);
            
            return true;
        } else if (used[neighbour] == Graph::WHITE_COLOUR) {
            prev[neighbour] = currentVertex;
            
            if (check_component_for_cycle(graph, neighbour, answer, used, prev)) {
                return true;
            }
        }
    }
    used[currentVertex] = Graph::BLACK_COLOUR;
    return false;
}

bool find_cycle(const Graph& graph, std::vector<vertex_t>& answer) {
    std::vector<bool> used(graph.get_vertexes_num() + 1, false);
    std::vector<vertex_t> prev(graph.get_vertexes_num() + 1, -1);

    for (vertex_t vertexToCheck = 0; vertexToCheck < graph.get_vertexes_num(); ++vertexToCheck) {
        if (used[vertexToCheck] == Graph::WHITE_COLOUR) {
            if (check_component_for_cycle(graph, vertexToCheck, answer, used, prev)) {
                return true;
            }
            answer.clear();
        }
    }
    return false;
}

int main() {
    int64_t vertexNum = 0;
    int64_t edgesNum = 0;

    std::cin >> vertexNum >> edgesNum;
    GraphList graph(vertexNum, edgesNum);

    for (size_t i = 0; i < edgesNum; ++i) {
        vertex_t first = 0;
        vertex_t second = 0;
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::vector<vertex_t> answer = {};
    if (find_cycle(graph, answer)) {
        std::cout << "YES" << std::endl;
        for (auto vertex : answer) {
            std::cout << vertex << ' ';
        }
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}