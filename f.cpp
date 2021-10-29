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
    virtual void insert_edge(const vertex_t& x, const vertex_t& y, const weight_t& weight = 0) = 0;
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

    void insert_edge(const vertex_t& x, const vertex_t& y,
                                          const weight_t& weight = 0) {}

    void add_new_line(std::vector<bool>& newLine) {
        cityMap_.push_back(newLine);
    }

    int32_t find_shortest_step(const Point& start, const Point& finish) {
        std::queue<Point> tempQueue;
        Point currentVertex = {0, 0};

        adjacencyTable_[start.x][start.y] = 0;
        tempQueue.push(start);
        
        while (!tempQueue.empty()) {
            currentVertex = tempQueue.front();

            tempQueue.pop();
            if (currentVertex == finish) {
                break;
            }

            for (auto step : steps) {
                Point delta = step;
                while ((currentVertex.x + delta.x >= 0 && currentVertex.x + delta.x < tableWide_ &&
                        currentVertex.y + delta.y >= 0 && currentVertex.y + delta.y < tableLenth_ &&
                        cityMap_[currentVertex.x + delta.x][currentVertex.y + delta.y])) {
                    delta.x += step.x;
                    delta.y += step.y;
                }
                delta.x /= 2;
                delta.y /= 2;
                if (adjacencyTable_[currentVertex.x + delta.x][currentVertex.y + delta.y] > adjacencyTable_[currentVertex.x][currentVertex.y] + 1) {
                    adjacencyTable_[currentVertex.x + delta.x][currentVertex.y + delta.y] = adjacencyTable_[currentVertex.x][currentVertex.y] + 1;
                    tempQueue.push({currentVertex.x + delta.x, currentVertex.y + delta.y});
                }
            }
        }

        if (adjacencyTable_[finish.x][finish.y] == kMaxValue_) {
            return -1;
        }
        return adjacencyTable_[finish.x][finish.y];
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
    std::vector<std::vector<bool>> cityMap_; // false - wall, true - track
    std::vector<std::vector<vertex_t>> adjacencyTable_;
    const int32_t kMaxValue_ = INT_MAX;
    const std::vector<Point> steps {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
};

int main() {
    int32_t tableWide = 0;
    int32_t tableLenth = 0;
    std::cin >> tableWide >> tableLenth;
    GraphTable graph(tableWide, tableLenth);

    char temp;
    std::vector<bool> line(tableLenth, false);
    Point start;
    Point finish;

    for (int32_t i = 0; i < tableWide; ++i) {
        for (int32_t j = 0; j < tableLenth; ++j) {
            std::cin >> temp;
            if (temp == 'S') {
                start = {i, j};
            }
            if (temp == 'T') {
                finish = {i, j};
            }
            line[j] = (temp != '#');
        }
        graph.add_new_line(line);
    }

    dist_t answer = graph.find_shortest_step(start, finish);
    std::cout << answer << std::endl;

    return 0;
}

