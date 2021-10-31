#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>
#include <set>
#include <map>

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

    struct Edge {
        vertex_t from_;
        vertex_t to_;

        friend bool operator<(const Edge& first, const Edge& second) {
            return first.from_ < second.from_ || (first.from_ == second.from_ && first.to_ < second.to_);
        }
    };

    struct EdgeStat {
        weight_t weight_;
        int32_t number_;
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
    virtual void insert_edge(const vertex_t& x, const vertex_t& y, const weight_t& weight = 0) = 0;
};

class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum) {
        vertexesNum_ = vertexesNum;

        adjacencyList_.resize(vertexesNum_, {});
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        Edge newEdge = {first - 1, second - 1};
        if (edges_.find(newEdge) != edges_.end()) {
            edges_[newEdge].number_++;
            return;
        }

        adjacencyList_[newEdge.from_].push_back(newEdge.to_);

        edges_[newEdge] = {weight, 1};
    }

    void create_euler_path(vertex_t currentVertex, vertex_t parent, std::vector<vertex_t>& answer) {
        for (auto neighbour : adjacencyList_[currentVertex]) {
            if (edges_[{currentVertex, neighbour}].number_ == 0) {
                continue;
            }

            --edges_[{currentVertex, neighbour}].number_;
            create_euler_path(neighbour, currentVertex, answer);
        }
        answer.push_back(currentVertex);
    }

    int32_t count_cycles() {
        int32_t result = 0;
        std::vector<int32_t> cycles(vertexesNum_, -1);
        for (vertex_t vertexToCheck = 0; vertexToCheck < vertexesNum_; ++vertexToCheck) {
            if (cycles[vertexToCheck] == -1 && adjacencyList_[vertexToCheck].size() != 0) {
                cycles[vertexToCheck] = vertexToCheck;
                check_vertex(vertexToCheck, vertexToCheck, cycles);
                result++;
            }
        }
        return result;
    }

protected: // useless in task
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
    std::vector<std::vector<vertex_t>> adjacencyList_; // using adjacency list
    std::map<Edge, EdgeStat> edges_;
    const int32_t kMaxValue_ = INT_MAX;

    void check_vertex(const vertex_t& currentVertex, const vertex_t& start, std::vector<int32_t>& cycles) {
        for (auto neighbour : adjacencyList_[currentVertex]) {
            if (cycles[neighbour] == -1) {
                cycles[neighbour] = start;
                check_vertex(neighbour, start, cycles);
            }
            if (cycles[neighbour] == start) {
                continue;
            }
        }
    }
};

int main() {
    int32_t vertexNum = 0;
    int32_t paths = 0;
    std::cin >> paths >> vertexNum;

    GraphList graph(vertexNum);
    vertex_t first = 0;
    vertex_t second = 0;
    int32_t length = 0;
    vertex_t minVertex = vertexNum;

    for (int32_t i = 0; i < paths; ++i) {
        std::cin >> length;
        std::cin >> first;
        for (int32_t j = 0; j < length; ++j) {
            std::cin >> second;
            graph.insert_edge(first, second);
            first = second;
            if (first < minVertex) {
                minVertex = first;
            }
        }
    }

    if (graph.count_cycles() != 1) {
        std::cout << 0 << std::endl;
        return 0;
    }

    std::vector<vertex_t> answer = {};

    graph.create_euler_path(minVertex - 1, 0, answer);
    std::reverse(answer.begin(), answer.end());
    if (answer.front() != answer.back()) {
        answer.clear();
    }
    std::cout << answer.size() << ' ';
    for (auto elem : answer) {
        std::cout << elem + 1 << ' ';
    }
    
    return 0;
}