#include <algorithm>
#include <iostream>
#include <vector>

typedef int64_t dist_t;
typedef int64_t vertex_t;
typedef int64_t weight_t;

class Graph {
protected:
    struct GraphNeighboursNode {
        weight_t weight_;
        vertex_t vertex_;
    };

    typedef enum {
        WHITE_COLOUR = 0,
        GREY_COLOUR = 1,
        BLACK_COLOUR = 2,
    } vertex_colours_t;

    size_t vertexesNum_;
    size_t edgesNum_;
    bool isOriented_;

    virtual std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) = 0;
    virtual std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) = 0;
    virtual std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) = 0;

public:
    virtual void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) = 0;
};

class GraphTable final : public Graph {
public:
    explicit GraphTable(size_t vertexesNum, size_t edgesNum) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        std::vector<weight_t> nullsLine(vertexesNum_, 0);
        adjacencyTable_.resize(vertexesNum_, nullsLine);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyTable_[first - 1][second - 1] = true;
        adjacencyTable_[second - 1][first - 1] = true;
    }

    bool is_bipartite() {
        std::vector<GraphTableNode> isVertexUsed(vertexesNum_, {false, WHITE_COLOUR});
        for (size_t currentVertex = 0; currentVertex < vertexesNum_; ++currentVertex) {
            if (!isVertexUsed[currentVertex].is_used) {
                isVertexUsed[currentVertex].is_used = true;
                isVertexUsed[currentVertex].colour = GREY_COLOUR;
                if (!check_connectivity_component(currentVertex, isVertexUsed)) {
                    return false;
                }
            }
        }
        return true;
    }

protected:
    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) {
        std::vector<vertex_t> answer = {};
        for (vertex_t currentVertex; currentVertex < vertexesNum_; ++currentVertex) {
            if (adjacencyTable_[vert][currentVertex]) {
                answer.push_back(currentVertex);
            }
        }
        return answer;
    }

    std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) {
        std::vector<weight_t> answer = {};
        for (vertex_t currentVertex; currentVertex < vertexesNum_; ++currentVertex) {
            answer.push_back(adjacencyTable_[vert][currentVertex]);
        }
        return answer;
    }

    std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) {
        std::vector<GraphNeighboursNode> answer = {};
        for (vertex_t currentVertex; currentVertex < vertexesNum_; ++currentVertex) {
            answer.push_back({currentVertex, adjacencyTable_[vert][currentVertex]});
        }
        return answer;
    }

private:
    struct GraphTableNode {
        bool is_used;
        vertex_colours_t colour;
    };

    std::vector<std::vector<weight_t>> adjacencyTable_;

    bool check_connectivity_component(size_t currentVertex, std::vector<GraphTableNode>& isVertexUsed) {
        vertex_colours_t currentColour = isVertexUsed[currentVertex].colour;
        bool answer = true;

        for (vertex_t vertexToCheck = 0; vertexToCheck < vertexesNum_; ++vertexToCheck) {
            if (adjacencyTable_[vertexToCheck][currentVertex]) {
                if (!isVertexUsed[vertexToCheck].is_used) {
                    isVertexUsed[vertexToCheck] = {true, (vertex_colours_t) (3 - currentColour)};
                    answer = check_connectivity_component(vertexToCheck, isVertexUsed);
                }
                if (isVertexUsed[vertexToCheck].colour == isVertexUsed[currentVertex].colour) {
                    return false;
                }
            }
        }
        return answer;
    }
};

int main() {
    int32_t vertexesNum = 0;
    int32_t edgesNum = 0;

    std::cin >> vertexesNum >> edgesNum;
    GraphTable graph(vertexesNum, edgesNum);

    vertex_t first = 0;
    vertex_t second = 0;
    for (size_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    if (graph.is_bipartite()) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}