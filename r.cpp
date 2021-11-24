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

void check_vertex_for_bridge(const Graph& graph, std::set<vertex_t>& answer, const vertex_t& currentVertex, const vertex_t& parent, int32_t& time,
                  std::vector<bool>& used, std::vector<int32_t>& time_in, std::vector<int32_t>& time_up) {
    int32_t children = 0;

    used[currentVertex] = true;
    time_in[currentVertex] = time;
    time_up[currentVertex] = time;
    ++time;

    for (auto neighbour : graph.get_neighbours_vertex(currentVertex)) {
        if (neighbour == parent) {
            continue;
        }
        if (used[neighbour]) {
            time_up[currentVertex] = std::min(time_up[currentVertex], time_in[neighbour]);
        } else {
            check_vertex_for_bridge(graph, answer, neighbour, currentVertex, time, used, time_in, time_up);
            time_up[currentVertex] = std::min(time_up[currentVertex], time_up[neighbour]);

            if ((time_up[neighbour] >= time_in[currentVertex]) && (parent != -1)) {
                answer.insert(currentVertex);
            }
            
            children++;
        }
    }

    if ((parent == -1) && (children > 1)) {
        answer.insert(currentVertex);
    }
}

std::set<vertex_t> find_articulation_points(const Graph& graph) {
    std::set<vertex_t> answer;

    std::vector<bool> used(graph.get_vertexes_num() + 1, false);
    std::vector<int32_t> time_up(graph.get_vertexes_num() + 1, 0);
    std::vector<int32_t> time_in(graph.get_vertexes_num() + 1, 0);
    int32_t time = 0;

    for (vertex_t currentVertex = 1; currentVertex < graph.get_vertexes_num() + 1; ++currentVertex) {
        if (!used[currentVertex]) {
            check_vertex_for_bridge(graph, answer, currentVertex, -1, time, used, time_in, time_up);
        }
    }

    return answer;
}

int main() {
    int32_t vertexNum = 0;
    int32_t edgesNum = 0;
    std::cin >> vertexNum >> edgesNum;

    GraphList graph(vertexNum, edgesNum);
    vertex_t first = 0;
    vertex_t second = 0;

    for (int32_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::set<vertex_t> answer = find_articulation_points(graph);

    std::cout << answer.size() << std::endl;
    for (auto vertex : answer) {
        std::cout << vertex << std::endl;
    }
    
    return 0;
}