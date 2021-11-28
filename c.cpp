#include <algorithm>
#include <iostream>
#include <queue>
#include <climits>
#include <set>

typedef int32_t dist_t;
typedef int32_t vertex_t;
typedef int32_t weight_t;

struct Point{
    int32_t x;
    int32_t y;

    bool operator==(const Point& second) {
        return x == second.x && y == second.y;
    }
};

class Graph {
public:
    struct GraphNeighboursNode {
        vertex_t vertex_;
        weight_t weight_;
    };

    struct Edge {
        vertex_t from_;
        vertex_t to_;
        weight_t weight_;
        int32_t number_;

        friend bool operator<(const Edge& first, const Edge& second) {
            return first.from_ < second.from_ || (first.from_ == second.from_ && first.to_ <= second.to_);
        }

        friend bool operator==(const Edge& first, const Edge& second) {
            return first.from_ == second.from_ && first.to_ == second.to_ && first.weight_ == second.weight_;
        }
    };

    typedef enum {
        WHITE_COLOUR,
        GREY_COLOUR,
        BLACK_COLOUR,
    } vertex_colours_t;
    
    size_t get_vertexes_num() const {
        return vertexesNum_;
    }

    size_t get_edges_num() const {
        return edgesNum_;
    }

    static const int32_t UNDEFINED;

    virtual std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) const = 0;
    virtual std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) const = 0;
    virtual std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) const = 0;
    virtual void insert_edge(const vertex_t& x, const vertex_t& y, const weight_t& weight = 0) = 0;

protected:
    size_t vertexesNum_;
    size_t edgesNum_;
    bool isOriented_ = false;
};

class GraphList final : public Graph {
public:
    explicit GraphList(size_t vertexesNum, size_t edgesNum = 0, bool isOriented = false) {
        vertexesNum_ = vertexesNum;
        edgesNum_ = edgesNum;
        isOriented_ = isOriented;
        adjacencyList_.resize(vertexesNum_ + 1, {});
    }

    void insert_edge(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        if (isOriented_) {
            insert_edge_oriented(first, second, weight);
        } else {
            insert_edge_not_oriented(first, second, weight);
        }
    }

    std::vector<vertex_t> get_neighbours_vertex(const vertex_t& vert) const {
        std::vector<vertex_t> answer(adjacencyList_[vert].size());
        for (size_t i = 0; i < answer.size(); ++i) {
            answer[i] = adjacencyList_[vert][i].to_;
        }
        return answer;
    }

    std::vector<weight_t> get_neighbours_weight(const vertex_t& vert) const {
        std::vector<weight_t> answer(adjacencyList_[vert].size());
        for (size_t i = 0; i < answer.size(); ++i) {
            answer[i] = adjacencyList_[vert][i].weight_;
        }
        return answer;
    }

    std::vector<GraphNeighboursNode> get_neighbours(const vertex_t& vert) const {
        std::vector<GraphNeighboursNode> answer(adjacencyList_[vert].size());
        for (size_t i = 0; i < answer.size(); ++i) {
            answer[i].vertex_ = adjacencyList_[vert][i].to_;
            answer[i].weight_ = adjacencyList_[vert][i].weight_;
        }
        return answer;
    }

private:
    std::vector<std::vector<Edge>> adjacencyList_; // using adjacency list

    void insert_edge_oriented(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyList_[first].push_back({first, second, weight});
    }

    void insert_edge_not_oriented(const vertex_t& first, const vertex_t& second, const weight_t& weight = 0) {
        adjacencyList_[first].push_back({first, second, weight});
        adjacencyList_[second].push_back({second, first, weight});
    }
};

const int32_t Graph::UNDEFINED = INT_MAX;

class Grid {
    int32_t size_of_field_;

public:
    explicit Grid(int32_t size) {
        size_of_field_ = size;
    }

    vertex_t convert_point_to_vertex(const Point& point) const {
        return (point.x - 1) * size_of_field_ + point.y;
    }

    Point convert_vertex_to_point(const vertex_t& vertex) const {
        Point answer;
        answer.x = (vertex - 1) / (size_of_field_) + 1;
        answer.y = (vertex - (answer.x - 1) * size_of_field_) % (size_of_field_ + 1);
        return answer;
    }

    bool check_is_inside(const Point& point) const {
        return !(point.x <= 0 || point.x > size_of_field_ || point.y <= 0 || point.y > size_of_field_);
    }

    size_t get_size() const {
        return size_of_field_;
    }
};

std::vector<vertex_t> normalize_path(const std::vector<vertex_t>& prev, const dist_t& length, const vertex_t& end) {
    std::vector<vertex_t> answer;
    vertex_t current = prev[end];
    answer.push_back(end);

    for (int32_t i = 0; i < length; ++i) {
        answer.push_back(current);
        current = prev[current];
    }

    std::reverse(answer.begin(), answer.end());
    return answer;
}

std::vector<vertex_t> find_shortest_path_with_saving(const Graph& graph,
                                                      const vertex_t& start,
                                                      const vertex_t& end) {
    std::vector<dist_t> dist(graph.get_vertexes_num() + 1, Graph::UNDEFINED);
    std::vector<vertex_t> prev(graph.get_vertexes_num() + 1, 0);
    std::queue<vertex_t> tempQueue;

    tempQueue.push(start);
    dist[start] = 0;

    vertex_t currentVertex = 0;

    while (!tempQueue.empty()) {
        currentVertex = tempQueue.front();
        tempQueue.pop();

        for (vertex_t currentNeighbour : graph.get_neighbours_vertex(currentVertex)) {
            if (dist[currentNeighbour] == Graph::UNDEFINED) {
                dist[currentNeighbour] = dist[currentVertex] + 1;
                prev[currentNeighbour] = currentVertex;

                tempQueue.push(currentNeighbour);
            }
        }
    }

    dist_t length = dist[end];
    if (length == Graph::UNDEFINED) {
        return {};
    }

    return normalize_path(prev, length, end);
}

void fill_graph(Graph& graph, const Grid& grid) {
    std::vector<Point> steps {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    
    for (int32_t i = 1; i < grid.get_size() + 1; ++i) {
        for (int32_t j = 1; j < grid.get_size() + 1; ++j) {
            Point currentPoint = {i, j};
            for (auto step : steps) {
                Point potentialNeighbour = {i + step.x, j + step.y};
                if (grid.check_is_inside(potentialNeighbour)) {
                    vertex_t vertexTo = grid.convert_point_to_vertex(currentPoint);
                    vertex_t vertexFrom = grid.convert_point_to_vertex(potentialNeighbour);
                    graph.insert_edge(vertexTo, vertexFrom);
                }
            }
        }
    }
}

std::vector<Point> find_path(const Graph& graph, const Grid& grid,
                             int32_t start_x, int32_t start_y,
                             int32_t end_x, int32_t end_y) {
    vertex_t start = grid.convert_point_to_vertex({start_x, start_y});
    vertex_t end = grid.convert_point_to_vertex({end_x, end_y});

    std::vector<vertex_t> pathVertexes = find_shortest_path_with_saving(graph, start, end);

    std::vector<Point> pathPoints;
    
    for (auto elem : pathVertexes) {
        pathPoints.push_back(grid.convert_vertex_to_point(elem));
    }

    return pathPoints;
}

int main() {
    int32_t vertexesNum = 0;
    std::cin >> vertexesNum;

    Grid grid(vertexesNum);

    int32_t start_x = 0;
    int32_t start_y = 0;
    int32_t end_x = 0;
    int32_t end_y = 0;
    std::cin >> start_x >> start_y;
    std::cin >> end_x >> end_y;

    GraphList graph(grid.convert_point_to_vertex({vertexesNum, vertexesNum}), 0, true);

    fill_graph(graph, grid);

    std::vector<Point> answer = find_path(graph, grid, start_x, start_y, end_x, end_y);

    std::cout << answer.size() - 1 << std::endl;
    for (auto elem : answer) {
        std::cout << elem.x << ' ' << elem.y << std::endl;
    }

    return 0;
}