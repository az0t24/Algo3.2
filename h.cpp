#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
#include <queue>

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
        WHITE_COLOUR = 0,
        GREY_COLOUR = 1,
        BLACK_COLOUR = 2,
    } vertex_colours_t;

    static const int32_t UNDEFINED;

    size_t get_vertexes_num() const {
        return vertexesNum_;
    }

    size_t get_edges_num() const {
        return edgesNum_;
    }

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

dist_t find_shortest_path_without_saving(const Graph& graph,
                                         const vertex_t& start,
                                         const vertex_t& end,
                                         const weight_t& max_weight) {
    std::vector<dist_t> dist(graph.get_vertexes_num() + 1, Graph::UNDEFINED);
    std::vector<std::queue<vertex_t>> dists_from_start(max_weight);

    dists_from_start[0].push(start);
    dist[start] = 0;

    for (weight_t currentWeight = 0; currentWeight <= max_weight * graph.get_vertexes_num(); currentWeight++) {
        while (!dists_from_start[currentWeight % max_weight].empty()) {
            vertex_t currentVertex = dists_from_start[currentWeight % max_weight].front();
            dists_from_start[currentWeight % max_weight].pop();

            for (auto currentNeighbour : graph.get_neighbours(currentVertex)) {
                if ((dist[currentNeighbour.vertex_] == Graph::UNDEFINED) ||
                    (dist[currentVertex] + currentNeighbour.weight_ < dist[currentNeighbour.vertex_])) {

                    dist[currentNeighbour.vertex_] = dist[currentVertex] + currentNeighbour.weight_;
                    dists_from_start[(currentNeighbour.weight_ + currentWeight) % max_weight].push(currentNeighbour.vertex_);
                }
            }
        }
    }

    dist_t length = dist[end];
    if (length == Graph::UNDEFINED) {
        return -1;
    }
    return length;
}

int main() {
    int32_t vertexNum = 0;
    int32_t edgesNum = 0;
    std::cin >> vertexNum >> edgesNum;

    vertex_t start = 0;
    vertex_t end = 0;
    std::cin >> start >> end;

    GraphList graph(vertexNum, edgesNum, true);

    for (int32_t i = 0; i < edgesNum; ++i) {
        vertex_t first = 0;
        vertex_t second = 0;
        weight_t value = 0;
        std::cin >> first >> second >> value;
        graph.insert_edge(first, second, value);
    }

    dist_t length = find_shortest_path_without_saving(graph, start, end, 11);

    std::cout << length << std::endl;
    
    return 0;
}