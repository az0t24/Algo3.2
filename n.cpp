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
protected:
    struct GraphNeighboursNode {
        vertex_t vertex_;
        weight_t weight_;
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

class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum, size_t edgesNum) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        edges_.resize(vertexesNum_, {});
        used_.resize(vertexesNum_, WHITE_COLOUR);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        edges_[first - 1].push_back(second - 1);
    }

    void top_sort(std::vector<vertex_t>& answer) {
        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_; ++currentVertex) {
            if (used_[currentVertex] == WHITE_COLOUR) {
                bool check_cycle = dfs_top_sort(answer, currentVertex);
                if (!check_cycle) {
                    answer.clear();
                    return;
                }
            }
        }
    }

protected: //useless in task
    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) {
        std::vector<vertex_t> answer = {};
        return answer;
    }

    std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) {
        std::vector<weight_t> answer = {};
        return answer;
    }

    std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) {
        std::vector<GraphNeighboursNode> answer = {};
        return answer;
    }

private:
    std::vector<std::vector<vertex_t>> edges_;
    std::vector<vertex_colours_t> used_;

    bool dfs_top_sort(std::vector<vertex_t>& answer, vertex_t currentVertex) {
        used_[currentVertex] = GREY_COLOUR;
        for (auto neighbour : edges_[currentVertex]) {
            if (used_[neighbour] == GREY_COLOUR) {
                return false;
            }
            if (used_[neighbour] == WHITE_COLOUR) {
                if (!dfs_top_sort(answer, neighbour)) {
                    return false;
                }
            }
            used_[neighbour] = BLACK_COLOUR;
        }
        used_[currentVertex] = BLACK_COLOUR;
        answer.push_back(currentVertex + 1);
        return true;
    }
};

int main() {
    int64_t vertexesNum = 0;
    int64_t edgesNum = 0;

    std::cin >> vertexesNum >> edgesNum;
    GraphList graph(vertexesNum, edgesNum);

    vertex_t first = 0;
    vertex_t second = 0;
    for (int32_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::vector<vertex_t> answer = {};
    graph.top_sort(answer);

    if (answer.size() == 0) {
        std::cout << -1 << std::endl;
        return 0;
    }
    std::reverse(answer.begin(), answer.end());
    for (int32_t i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << ' ';
    }
    
    return 0;
}