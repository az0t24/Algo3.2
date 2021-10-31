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
                dfs_top_sort(answer, currentVertex);
            }
        }
    }

    int64_t count_comps(std::vector<vertex_t>& answer, std::vector<vertex_t>& sorted) {
        int64_t compNum = 1;
        for (int32_t i = 0; i < vertexesNum_; ++i) {
            vertex_t currentVertex = sorted[i];
            if (used_[currentVertex] == WHITE_COLOUR) {
                find_members_of_comp(currentVertex);
                for (auto elem : components_) {
                    answer[elem] = compNum;
                }
                compNum++;
                components_.clear();
            }
        }
        return compNum;
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
    std::vector<std::vector<int64_t>> edges_;
    std::vector<vertex_colours_t> used_;
    std::vector<int64_t> components_;

    void find_members_of_comp(vertex_t currentVertex) {
        used_[currentVertex] = GREY_COLOUR;
        components_.push_back(currentVertex);
        for (auto neighbour : edges_[currentVertex]) {
            if (used_[neighbour] == 0) {
                find_members_of_comp(neighbour);
            }
        }
    }

    void dfs_top_sort(std::vector<vertex_t>& answer, vertex_t currentVertex) {
        used_[currentVertex] = GREY_COLOUR;
        for (auto neighbour : edges_[currentVertex]) {
            if (used_[neighbour] == WHITE_COLOUR) {
                dfs_top_sort(answer, neighbour);
            }
        }
        answer.push_back(currentVertex);
    }
};

int main() {
    int64_t vertexesNum = 0;
    int64_t edgesNum = 0;

    std::cin >> vertexesNum >> edgesNum;
    GraphList graph(vertexesNum, edgesNum);
    GraphList transponed_graph(vertexesNum, edgesNum);

    vertex_t first = 0;
    vertex_t second = 0;
    for (int64_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
        transponed_graph.insert_edge(second, first);
    }

    std::vector<vertex_t> topology_sorted_vertexes = {};
    graph.top_sort(topology_sorted_vertexes);

    std::reverse(topology_sorted_vertexes.begin(), topology_sorted_vertexes.end());
    
    std::vector<int64_t> answer(vertexesNum);
    std::cout << transponed_graph.count_comps(answer, topology_sorted_vertexes) - 1 << std::endl;

    for (size_t i = 0; i < answer.size(); ++i) {
        std::cout << answer[i] << ' ';
    }
    
    return 0;
}