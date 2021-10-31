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
        used_.resize(vertexesNum_, false);
        comp_.resize(vertexesNum_, {});
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        edges_[first - 1].push_back(second - 1);
        edges_[second - 1].push_back(first - 1);
    }

    int32_t find_components() {
        int32_t result = 0;
        for (vertex_t vertexToCheck = 0; vertexToCheck < vertexesNum_; ++vertexToCheck) {
            if (!used_[vertexToCheck]) {
                check_vertex(vertexToCheck, result);
                result++;
            }
        }
        return result;
    }

    void print_answer() {
        int32_t num = find_components();
        std::cout << num << std::endl;
        for (int32_t currentComp = 0; currentComp < num; ++currentComp) {
            std::cout << comp_[currentComp].size() << std::endl;
            for (vertex_t currentVertex = 0; currentVertex < comp_[currentComp].size(); ++currentVertex) {
                std::cout << comp_[currentComp][currentVertex] + 1 << ' ';
            }
            std::cout << std::endl;
        }
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
    std::vector<std::vector<vertex_t>> edges_;
    std::vector<bool> used_;
    std::vector<std::vector<vertex_t>> comp_;

    void check_vertex(vertex_t currentVertex, int32_t componentNum) {
        used_[currentVertex] = true;
        comp_[componentNum].push_back(currentVertex);
        for (auto neighbour : edges_[currentVertex]) {
            if (!used_[neighbour]) {
                check_vertex(neighbour, componentNum);
            }
        }
    }
};

int main() {
    int64_t N = 0;
    int64_t M = 0;

    std::cin >> N >> M;
    GraphList graph(N, M);

    int64_t first = 0;
    int64_t second = 0;
    for (int64_t i = 0; i < M; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    graph.print_answer();
    
    return 0;
}