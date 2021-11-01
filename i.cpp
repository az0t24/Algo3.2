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
        edges_.resize(vertexesNum_);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        edges_[first - 1].push_back({second - 1, weight});
    }

    dist_t finding_shortest_path_without_saving(const vertex_t& start, const vertex_t& end) {
        std::vector<dist_t> dist(vertexesNum_, kMaxDistance_);
        std::vector<std::queue<vertex_t>> dists_from_start(kMaxWeight_);

        dists_from_start[0].push(start - 1);
        vertex_t currentVertex = 0;
        dist[start - 1] = 0;

        for (weight_t currentWeight = 0; currentWeight <= kMaxWeight_ * vertexesNum_; currentWeight++) {
            while (!dists_from_start[currentWeight % kMaxWeight_].empty()) {
                currentVertex = dists_from_start[currentWeight % kMaxWeight_].front();
                dists_from_start[currentWeight % kMaxWeight_].pop();

                for (auto currentNeighbour : edges_[currentVertex]) {
                    if ((dist[currentNeighbour.vertex_] == kMaxDistance_) ||
                        (dist[currentVertex] + currentNeighbour.weight_ < dist[currentNeighbour.vertex_])) {

                        dist[currentNeighbour.vertex_] = dist[currentVertex] + currentNeighbour.weight_;
                        dists_from_start[(currentNeighbour.weight_ + currentWeight) % kMaxWeight_].push(currentNeighbour.vertex_);
                    }
                }
            }
        }

        if (dist[end - 1] == kMaxDistance_) {
            return -1;
        }

        return dist[end - 1];
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
    std::vector<std::vector<GraphNeighboursNode>> edges_; // using adjacency list
    const int32_t kMaxDistance_ = INT_MAX;
    const int32_t kMaxWeight_ = 31;
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

    std::cout << graph.finding_shortest_path_without_saving(start, end) << std::endl;
    
    return 0;
}