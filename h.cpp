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
        extraVertexesNum_ = vertexesNum_;
        edges_.resize(vertexesNum_, {});
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        if (weight == 1) {
            edges_[first - 1].push_back(second - 1);
        }
        edges_[first - 1].push_back(extraVertexesNum_);
        extraVertexesNum_++;
        for (int32_t i = 0; i < weight - 2; ++i) {
            edges_.push_back({extraVertexesNum_});
            extraVertexesNum_++;
        }
        edges_.push_back({second - 1});
    }

    dist_t finding_shortest_path_without_saving(const vertex_t& start, const vertex_t& end) {
        std::vector<dist_t> dist(extraVertexesNum_, kMaxValue_);
        std::deque<vertex_t> tempDeque;

        tempDeque.push_back(start - 1);
        dist[start - 1] = 0;
        vertex_t currentVertex = 0;

        while (!tempDeque.empty()) {
            currentVertex = tempDeque.front();
            tempDeque.pop_front();

            for (auto currentNeighbour : edges_[currentVertex]) {
                if (dist[currentNeighbour] > dist[currentVertex] + 1) {
                    dist[currentNeighbour] = dist[currentVertex] + 1;

                    tempDeque.push_back(currentNeighbour);
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
    int32_t extraVertexesNum_;
    std::vector<std::vector<vertex_t>> edges_; // using adjacency list
    const int32_t kMaxValue_ = INT_MAX;
};

int main() {
    int32_t vertexNum = 0;
    int32_t edgesNum = 0;
    std::cin >> vertexNum >> edgesNum;

    vertex_t start = 0;
    vertex_t end = 0;
    std::cin >> start >> end;

    GraphList graph(vertexNum, edgesNum);
    vertex_t first = 0;
    vertex_t second = 0;
    weight_t value = 0;
    for (int32_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second >> value;
        graph.insert_edge(first, second, value);
    }

    dist_t length = graph.finding_shortest_path_without_saving(start, end);

    std::cout << length << std::endl;
    
    return 0;
}