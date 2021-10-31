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
    explicit GraphList(size_t vertexesNum) {
        vertexesNum_ = vertexesNum;
        edges_.resize(vertexesNum_, {});
        used_.resize(vertexesNum_, WHITE_COLOUR);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        edges_[first].push_back(second);
    }

    bool find_way() {
        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_; ++currentVertex) {
            if (used_[currentVertex] == WHITE_COLOUR) {
                bool check_cycle = find_cycle(currentVertex);
                if (!check_cycle) {
                    return true;
                }
            }
        }
        return false;
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

    bool find_cycle(vertex_t currentVertex) {
        used_[currentVertex] = GREY_COLOUR;
        for (auto neighbour : edges_[currentVertex]) {
            if (used_[neighbour] == GREY_COLOUR) {
                return false;
            }
            if (used_[neighbour] == WHITE_COLOUR) {
                if (!find_cycle(neighbour)) {
                    return false;
                }
            }
            used_[neighbour] = BLACK_COLOUR;
        }
        used_[currentVertex] = BLACK_COLOUR;
        return true;
    }
};

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    int64_t N = 0;
    int64_t M = 0;

    std::cin >> N;
    GraphList graph(N);

    std::string data;
    for (int64_t i = 0; i < N - 1; ++i) {
        std::cin >> data;
        for (int64_t j = 0; j < N - 1 - i; ++j) {
            if (data[j] == 'R') {
                graph.insert_edge(i, i + j + 1);
            } else {
                graph.insert_edge(i + j + 1, i);
            }
        }
    }

    if (graph.find_way()) {
        std::cout << "NO" << std::endl;
    } else {
        std::cout << "YES" << std::endl;
    }
    
    return 0;
}