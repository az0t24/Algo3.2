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
        adjacencyList_.resize(vertexesNum_, {});
        used_.resize(vertexesNum_, WHITE_COLOUR);
        prev_.resize(vertexesNum_, -1);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyList_[first - 1].insert(second - 1);
    }

    bool find_cycle(std::vector<vertex_t>& answer) {
        for (vertex_t vertexToCheck = 0; vertexToCheck < vertexesNum_; ++vertexToCheck) {
            if (used_[vertexToCheck] == WHITE_COLOUR) {
                if (check_vertex(vertexToCheck, answer)) {
                    return true;
                }
                answer.clear();
            }
        }
        return false;
    }

protected:
    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) {
        std::vector<vertex_t> answer = {};
        for (auto elem : adjacencyList_[vert]) {
            answer.push_back(elem);
        }
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
    std::vector<vertex_colours_t> used_;
    std::vector<std::set<vertex_t>> adjacencyList_;
    std::vector<vertex_t> prev_;

    bool check_vertex(const vertex_t& currentVertex, std::vector<vertex_t>& answer) {
        used_[currentVertex] = GREY_COLOUR;
        
        for (auto neighbour : adjacencyList_[currentVertex]) {
            if (used_[neighbour] == GREY_COLOUR) {
                vertex_t start = neighbour;
                vertex_t end = currentVertex;

                while (end != start) {
                    answer.push_back(end + 1);
                    end = prev_[end];
                }
                
                answer.push_back(end + 1);
                std::reverse(answer.begin(), answer.end());
                
                return true;
            } else if (used_[neighbour] == WHITE_COLOUR) {
                prev_[neighbour] = currentVertex;
                
                if (check_vertex(neighbour, answer)) {
                    return true;
                }
            }
        }
        used_[currentVertex] = BLACK_COLOUR;
        return false;
    }
};

int main() {
    int64_t vertexNum = 0;
    int64_t edgesNum = 0;

    std::cin >> vertexNum >> edgesNum;
    GraphList graph(vertexNum, edgesNum);

    vertex_t first = 0;
    vertex_t second = 0;
    for (int64_t i = 0; i < edgesNum; ++i) {
        std::cin >> first >> second;
        graph.insert_edge(first, second);
    }

    std::vector<vertex_t> answer = {};
    if (graph.find_cycle(answer)) {
        std::cout << "YES" << std::endl;
        for (size_t i = 0; i < answer.size(); ++i) {
            std::cout << answer[i] << ' ';
        }
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}