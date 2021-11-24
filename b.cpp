#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

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

    bool isOriented_ = false;

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
};

class GraphTable final : public Graph {
public:
    explicit GraphTable(size_t vertexesNum, size_t edgesNum, bool isOriented = false) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        isOriented_ = isOriented;
        std::vector<weight_t> nullsLine(vertexesNum_ + 1, 0);
        adjacencyTable_.resize(vertexesNum_ + 1, nullsLine);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 1) {
        if (isOriented_) {
            insert_edge_oriented(first, second, weight);
        } else {
            insert_edge_not_oriented(first, second, weight);
        }
    }

    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) const {
        std::vector<vertex_t> answer = {};
        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_ + 1; ++currentVertex) {
            if (adjacencyTable_[vert][currentVertex] > 0) {
                answer.push_back(currentVertex);
            }
        }
        return answer;
    }

    std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) const {
        std::vector<weight_t> answer = {};
        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_ + 1; ++currentVertex) {
            answer.push_back(adjacencyTable_[vert][currentVertex] > 0);
        }
        return answer;
    }

    std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) const {
        std::vector<GraphNeighboursNode> answer = {};
        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_ + 1; ++currentVertex) {
            answer.push_back({currentVertex, adjacencyTable_[vert][currentVertex]});
        }
        return answer;
    }

private:
    std::vector<std::vector<weight_t>> adjacencyTable_;

    void insert_edge_oriented(const vertex_t& first, const vertex_t& second, const weight_t& weight = 1) {
        adjacencyTable_[first][second] = weight;
    }

    void insert_edge_not_oriented(const vertex_t& first, const vertex_t& second, const weight_t& weight = 1) {
        adjacencyTable_[first][second] = weight;
        adjacencyTable_[second][first] = weight;
    }
};

const int32_t Graph::UNDEFINED = INT_MAX;

Graph::vertex_colours_t swap_colour(const Graph::vertex_colours_t currentColour) {
    return (currentColour == Graph::GREY_COLOUR) ? Graph::BLACK_COLOUR : Graph::GREY_COLOUR;
}

bool check_neighbours_colour(const Graph& graph, const vertex_t& currentVertex, std::vector<Graph::vertex_colours_t>& vertexColour) {
    Graph::vertex_colours_t currentColour = vertexColour[currentVertex];
    bool is_comp_ok = true;

    for (vertex_t currentNeighbour : graph.get_neighbours_vertex(currentVertex)) {
        if (vertexColour[currentNeighbour] == Graph::WHITE_COLOUR) {
            vertexColour[currentNeighbour] = swap_colour(currentColour);
            is_comp_ok = check_neighbours_colour(graph, currentNeighbour, vertexColour);
        }
        if (vertexColour[currentNeighbour] == currentColour) {
            return false;
        }
    }
    return is_comp_ok;
}

bool is_bipartite(const Graph& graph) {
    std::vector<Graph::vertex_colours_t> vertexColour(graph.get_vertexes_num() + 1, Graph::WHITE_COLOUR);
    for (vertex_t currentVertex = 1; currentVertex < graph.get_vertexes_num() + 1; ++currentVertex) {
        if (vertexColour[currentVertex] == Graph::WHITE_COLOUR) {
            vertexColour[currentVertex] = Graph::GREY_COLOUR;
            if (!check_neighbours_colour(graph, currentVertex, vertexColour)) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    int32_t vertexesNum = 0;
    int32_t edgesNum = 0;

    std::cin >> vertexesNum >> edgesNum;
    GraphTable graph(vertexesNum, edgesNum);

    for (size_t i = 0; i < edgesNum; ++i) {
        vertex_t first = 0;
        vertex_t second = 0;
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    if (is_bipartite(graph)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}