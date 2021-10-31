#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>

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
    virtual void insert_edge(const vertex_t& x, const vertex_t& y, const weight_t& weight = 0) = 0;
};

class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum, size_t edgesNum) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        edges_.resize(vertexesNum_, {});
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        edges_[first - 1].push_back({second - 1, 0});
        edges_[second - 1].push_back({first - 1, 1});
    }

    dist_t finding_shortest_path_without_saving(const vertex_t& start,
                                                 const vertex_t& end) {
        std::vector<dist_t> dist(vertexesNum_, kMaxValue_);
        std::vector<vertex_t> prev(vertexesNum_, 0);
        std::deque<vertex_t> tempDeque;
        tempDeque.push_back(start - 1);
        dist[start - 1] = 0;
        vertex_t currentVertex = 0;

        while (!tempDeque.empty()) {
            currentVertex = tempDeque.front();
            tempDeque.pop_front();

            for (auto neighbour : get_neighbours(currentVertex)) {
                if (dist[neighbour.vertex_] > dist[currentVertex] + neighbour.weight_) {
                    if (!neighbour.weight_) {
                        dist[neighbour.vertex_] = dist[currentVertex];
                        tempDeque.push_front(neighbour.vertex_);
                    } else {
                        dist[neighbour.vertex_] = dist[currentVertex] + 1;
                        tempDeque.push_back(neighbour.vertex_);
                    }
                }
            }
        }
        if (dist[end - 1] == kMaxValue_) {
            return -1;
        }

        dist_t length = dist[end - 1];

        return length;
    }

protected:
    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) {
        std::vector<vertex_t> answer = {};
        for (auto elem : edges_[vert]) {
            answer.push_back(elem.vertex_);
        }
        return answer;
    }

    std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) {
        std::vector<weight_t> answer = {};
        for (auto elem : edges_[vert]) {
            answer.push_back(elem.weight_);
        }
        return answer;
    }

    std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) {
        std::vector<GraphNeighboursNode> answer = edges_[vert];
        return answer;
    }
    
    
private:
    std::vector<std::vector<GraphNeighboursNode>> edges_; // using adjacency list
    const int32_t kMaxValue_ = INT_MAX;
};

int main() {
    int32_t vertexNum = 0;
    int32_t edgesNum = 0;
    std::cin >> vertexNum >> edgesNum;

    vertex_t start = 0;
    vertex_t end = 0;

    GraphList graph(vertexNum, edgesNum);
    vertex_t first = 0;
    vertex_t second = 0;
    for (int32_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    int32_t queries = 0;
    std::cin >> queries;
    for (int32_t i = 0; i < queries; ++i) {
        std::cin >> start >> end;
        dist_t lenth = graph.finding_shortest_path_without_saving(start, end);

        std::cout << lenth << std::endl;
    }

    return 0;
}