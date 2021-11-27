#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>
#include <set>

typedef int64_t dist_t;
typedef int64_t vertex_t;
typedef int64_t weight_t;

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
    explicit GraphList(size_t vertexesNum, size_t edgesNum = 0, bool isOriented = false) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        isOriented_ = isOriented;
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

void dfs_top_sort(const Graph& graph, std::vector<vertex_t>& answer, const vertex_t& currentVertex,
                  std::vector<bool>& used) {
    used[currentVertex] = true;
    for (auto neighbour : graph.get_neighbours_vertex(currentVertex)) {
        if (used[neighbour] == false) {
            dfs_top_sort(graph, answer, neighbour, used);
        }
    }
    answer.push_back(currentVertex);
}

void top_sort(const Graph& graph, std::vector<vertex_t>& answer) {
    std::vector<bool> used(graph.get_vertexes_num() + 1, false);
    for (vertex_t currentVertex = 1; currentVertex < graph.get_vertexes_num() + 1; ++currentVertex) {
        if (used[currentVertex] == false) {
            dfs_top_sort(graph, answer, currentVertex, used);
        }
    }
}

void find_members_of_comp(const Graph& graph, const vertex_t& currentVertex,
                          std::vector<bool>& used, std::vector<int32_t>& components) {
    used[currentVertex] = true;
    components.push_back(currentVertex);
    for (auto neighbour : graph.get_neighbours_vertex(currentVertex)) {
        if (used[neighbour] == false) {
            find_members_of_comp(graph, neighbour, used, components);
        }
    }
}

int32_t count_comps(const Graph& graph, std::vector<int32_t>& answer, const std::vector<vertex_t>& sorted) {
    int32_t compNum = 1;
    std::vector<bool> used(graph.get_vertexes_num() + 1, false);
    std::vector<int32_t> components;
    for (int32_t i = 0; i < graph.get_vertexes_num(); ++i) {
        vertex_t currentVertex = sorted[i];
        if (used[currentVertex] == false) {
            find_members_of_comp(graph, currentVertex, used, components);
            for (auto elem : components) {
                answer[elem] = compNum;
            }
            compNum++;
            components.clear();
        }
    }
    return compNum;
}

GraphList make_transponed(const Graph& graph) {
    GraphList answer(graph.get_vertexes_num(), graph.get_edges_num(), true);

    for (vertex_t currentVertex = 1; currentVertex < graph.get_vertexes_num() + 1; currentVertex++) {
        for (auto neighbour : graph.get_neighbours_vertex(currentVertex)) {
            answer.insert_edge(neighbour, currentVertex);
        }
    }

    return answer;
}

size_t find_strong_comps(const Graph& graph, std::vector<int32_t>& answer) {
    std::vector<vertex_t> topology_sorted_vertexes = {};

    GraphList transponed_graph = make_transponed(graph);

    top_sort(graph, topology_sorted_vertexes);

    std::reverse(topology_sorted_vertexes.begin(), topology_sorted_vertexes.end());
    
    return count_comps(transponed_graph, answer, topology_sorted_vertexes) - 1;

}

int main() {
    int64_t vertexesNum = 0;
    int64_t edgesNum = 0;

    std::cin >> vertexesNum >> edgesNum;
    GraphList graph(vertexesNum, edgesNum, true);

    for (int64_t i = 0; i < edgesNum; ++i) {
        vertex_t first = 0;
        vertex_t second = 0;
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::vector<int32_t> answer(graph.get_vertexes_num() + 1);
    std::cout << find_strong_comps(graph, answer) << std::endl;

    for (size_t i = 1; i < answer.size(); ++i) {
        std::cout << answer[i] << ' ';
    }
    
    return 0;
}