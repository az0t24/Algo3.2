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
        edges_.resize(vertexesNum_, -1);
        cycles_.resize(vertexesNum_, -1);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        edges_[first - 1] = second - 1;
    }

    int32_t count_cycles() {
        int32_t result = 0;
        for (vertex_t vertexToCheck = 0; vertexToCheck < vertexesNum_; ++vertexToCheck) {
            if (cycles_[vertexToCheck] == -1) {
                cycles_[vertexToCheck] = vertexToCheck;
                result += check_vertex(edges_[vertexToCheck], vertexToCheck);
            }
        }
        return result;
    }

protected:
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
    std::vector<vertex_t> edges_;
    std::vector<vertex_t> cycles_;

    bool check_vertex(const vertex_t& currentVertex, const vertex_t& start) {
        if (cycles_[currentVertex] == -1) {
            cycles_[currentVertex] = start;
            return check_vertex(edges_[currentVertex], start);
        }
        if (cycles_[currentVertex] == start) {
            return 1;
        }
        return 0;

    }
};

int main() {
    int64_t vertexesNum = 0;

    std::cin >> vertexesNum;
    GraphList graph(vertexesNum);

    vertex_t key = 0;
    for (vertex_t currentVertex = 1; currentVertex <= vertexesNum; ++currentVertex) {
        std::cin >> key;
        graph.insert_edge(currentVertex, key);
    }

    std::cout << graph.count_cycles();

    return 0;
}