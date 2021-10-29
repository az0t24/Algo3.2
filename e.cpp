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
};

class Graph {
protected:
    struct GraphNeighboursNode {
        weight_t weight_;
        vertex_t vertex_;
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

class GraphTable final : public Graph {
public:
    explicit GraphTable(size_t tableWide, size_t tableLenth) {
        tableWide_ = tableWide;
        tableLenth_ = tableLenth;
        std::vector<vertex_t> nullsLine(tableLenth_, kMaxValue_);
        adjacencyTable_.resize(tableWide_, nullsLine);
        cityMap_ = {};
    }

    void insert_edge(const vertex_t& first, const vertex_t& second,
                                          const weight_t& weight = 0) {}

    void add_new_line(std::vector<bool>& newLine) {
        cityMap_.push_back(newLine);
    }

    void find_shortest_path_to_subs() {
        std::queue<Point> tempQueue;
        Point currentPoint = {0, 0};

        find_all_subs(tempQueue);
        
        while (!tempQueue.empty()) {
            currentPoint = tempQueue.front();
            tempQueue.pop();
            std::vector<Point> steps {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            for (auto step : steps) {
                if (check_boards(step, currentPoint)) {
                    continue;
                }
                if (adjacencyTable_[currentPoint.x + step.x][currentPoint.y + step.y] > adjacencyTable_[currentPoint.x][currentPoint.y] + 1) {
                    adjacencyTable_[currentPoint.x + step.x][currentPoint.y + step.y] = adjacencyTable_[currentPoint.x][currentPoint.y] + 1;
                    tempQueue.push({currentPoint.x + step.x, currentPoint.y + step.y});
                }
            }
        }
    }

    void print_answer() {
        for (int32_t i = 0; i < tableWide_; ++i) {
            for (int32_t j = 0; j < tableLenth_; ++j) {
                std::cout << adjacencyTable_[i][j] << ' ';
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
    size_t tableWide_;
    size_t tableLenth_;
    std::vector<std::vector<bool>> cityMap_;
    std::vector<std::vector<vertex_t>> adjacencyTable_;
    const int32_t kMaxValue_ = INT_MAX;

    void find_all_subs(std::queue<Point>& tempQueue) {
        for (int32_t i = 0; i < tableWide_; ++i) {
            for (int32_t j = 0; j < tableLenth_; ++j) {
                if (cityMap_[i][j]) {
                    adjacencyTable_[i][j] = 0;
                    tempQueue.push({i, j});
                }
            }
        }
    }

    bool check_boards(const Point& first, const Point& second) {
        return (first.x + second.x < 0 || first.x + second.x >=tableWide_ ||
                first.y + second.y < 0 || first.y + second.y >= tableLenth_);
    }
};

int main() {
    int32_t tableWide = 0;
    int32_t tableLenth = 0;
    std::cin >> tableWide >> tableLenth;
    GraphTable graph(tableWide, tableLenth);
    bool temp = false;
    std::vector<bool> line(tableLenth, false);
    for (size_t i = 0; i < tableWide; ++i) {
        for (size_t j = 0; j < tableLenth; ++j) {
            std::cin >> temp;
            line[j] = temp;
        }
        graph.add_new_line(line);
    }

    graph.find_shortest_path_to_subs();
    graph.print_answer();

    return 0;
}