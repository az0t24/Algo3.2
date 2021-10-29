#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
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

class GraphList final : public Graph {
public:
  explicit GraphList(int32_t vertexesNum) {
    vertexesNum_ = vertexesNum;
  }

  void insert_edge(const vertex_t& first, const vertex_t& second,
                                          const weight_t& weight = 0) {}

  dist_t finding_shortest_path_with_saving(std::vector<Point>& answer,
                                          const Point& start, const Point& end) {
        std::vector<dist_t> nullDistLine(vertexesNum_, kMaxDist_);
        std::vector<std::vector<dist_t>> dist(vertexesNum_, nullDistLine);

        std::vector<Point> nullPointLine(vertexesNum_, {0, 0});
        std::vector<std::vector<Point>> prev(vertexesNum_, nullPointLine);

        std::queue<Point> tempQueue;
        tempQueue.push({start.x - 1, start.y - 1});
        dist[start.x - 1][start.y - 1] = 0;

        Point current = {0, 0};

        while (!tempQueue.empty()) {
            current = tempQueue.front();
            tempQueue.pop();

            std::vector<Point> steps {{1, 2}, {1, -2}, {-1, 2}, {-1, -2},
                                      {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
            for (auto step : steps) {
              if (check_boards(current, step)) {
                continue;
              }
                if (dist[step.x + current.x][step.y + current.y] == kMaxDist_) {
                    dist[step.x + current.x][step.y + current.y] = dist[current.x][current.y] + 1;
                    prev[step.x + current.x][step.y + current.y] = current;
                    tempQueue.push({step.x + current.x, step.y + current.y});
                }
            }
        }
        if (dist[end.x - 1][end.y - 1] == kMaxDist_) {
            return 0;
        }

        int32_t length = dist[end.x - 1][end.y - 1];
        Point last = prev[end.x - 1][end.y - 1];
        answer.push_back(end);
        for (int32_t i = 0; i < length; ++i) {
            answer.push_back({last.x + 1, last.y + 1});
            last = prev[last.x][last.y];
        }

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
    const dist_t kMaxDist_ = INT_MAX;

    bool check_boards(const Point& first, const Point& second) {
        return (first.x + second.x < 0 || first.y + second.y < 0 ||
                first.y + second.y >= vertexesNum_ || first.x + second.x >= vertexesNum_);
    }
};

int main() {
    int32_t vertexesNum = 0;
    std::cin >> vertexesNum;

    int32_t start_x = 0;
    int32_t start_y = 0;
    int32_t end_x = 0;
    int32_t end_y = 0;
    std::cin >> start_x >> start_y;
    std::cin >> end_x >> end_y;

    GraphList graph(vertexesNum);

    std::vector<Point> answer = {};
    int length = graph.finding_shortest_path_with_saving(answer, {start_x, start_y}, {end_x, end_y});

    std::cout << length << std::endl;
    for (size_t i = 0; i < length + 1; ++i) {
        std::cout << answer.back().x << " " << answer.back().y << std::endl;
        answer.pop_back();
    }

    return 0;
}