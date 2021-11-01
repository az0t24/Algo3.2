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
    explicit GraphList(size_t vertexesNum, size_t edgesNum) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        adjacencyList_.resize(vertexesNum_, {});
        used_.resize(vertexesNum_, false);
        time_up_.resize(vertexesNum_, 0);
        time_in_.resize(vertexesNum_, 0);
        time_ = 0;
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyList_[first - 1].push_back(second - 1);
        adjacencyList_[second - 1].push_back(first - 1);
    }

    int32_t count_extra_edges() {
        int32_t answer = 0;

        std::vector<std::vector<vertex_t>> comps(vertexesNum_);
        std::vector<int32_t> numOfComps(vertexesNum_, -1);

        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_; ++currentVertex) {
            if (!used_[currentVertex]) {
                check_vertex_dfs(currentVertex, -1);
            }
        }

        find_double_comps(comps, numOfComps);

        for (auto comp : comps) {
            int32_t bridges = 0;
            for (auto currentVertex : comp) {
                for (auto neighbour : adjacencyList_[currentVertex]) {
                    if (numOfComps[neighbour] != numOfComps[currentVertex]) {
                        bridges++;
                    }
                }
            }

            if (bridges == 1) {
                answer++;
            }
        }

        return (answer + 1) / 2;
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
    std::vector<std::vector<vertex_t>> adjacencyList_; // using adjacency list
    std::vector<bool> used_;
    std::vector<int32_t> time_up_;
    std::vector<int32_t> time_in_;
    int32_t time_;
    const int32_t kMaxValue_ = INT_MAX;

    void check_vertex_dfs(vertex_t currentVertex, vertex_t parent) {
        used_[currentVertex] = true;
        time_in_[currentVertex] = time_;
        time_up_[currentVertex] = time_;
        ++time_;

        for (auto neighbour : adjacencyList_[currentVertex]) {
            if (neighbour == parent) {
                continue;
            }
            if (used_[neighbour]) {
                time_up_[currentVertex] = std::min(time_up_[currentVertex], time_in_[neighbour]);
            } else {
                check_vertex_dfs(neighbour, currentVertex);
                time_up_[currentVertex] = std::min(time_up_[currentVertex], time_up_[neighbour]);
            }
        }
    }

    void fill_comp(vertex_t currentVertex, int32_t currentComp, int32_t& maxComp, std::vector<std::vector<vertex_t>>& comps, std::vector<int32_t>& numOfComps) {
        comps[currentComp].push_back(currentVertex);
        numOfComps[currentVertex] = currentComp;

        for (auto neighbour : adjacencyList_[currentVertex]) {
            if (numOfComps[neighbour] == -1) {
                if (time_up_[neighbour] > time_in_[currentVertex]) {
                    maxComp++;
                    fill_comp(neighbour, maxComp, maxComp, comps, numOfComps);
                } else {
                    fill_comp(neighbour, currentComp, maxComp, comps, numOfComps);
                }
            }
        }
    }

    void find_double_comps(std::vector<std::vector<vertex_t>>& comps, std::vector<int32_t>& numOfComps) {
        int32_t currentComp = -1;

        for (vertex_t currentVertex = 0; currentVertex < vertexesNum_; ++currentVertex) {
            if (numOfComps[currentVertex] == -1) {
                currentComp++;
                fill_comp(currentVertex, currentComp, currentComp, comps, numOfComps);
            }
        }
    }
};

int main() {
    int32_t vertexNum = 0;
    int32_t edgesNum = 0;
    std::cin >> vertexNum >> edgesNum;

    GraphList graph(vertexNum, edgesNum);
    vertex_t first = 0;
    vertex_t second = 0;

    for (int32_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::cout << graph.count_extra_edges();
    
    return 0;
}