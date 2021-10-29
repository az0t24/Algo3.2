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

class GraphList final : public Graph {
public:
  explicit GraphList(size_t vertexesNum) {
    vertexesNum_ = vertexesNum;
  }

    void insert_edge(const vertex_t& first, const vertex_t& second,
                                          const weight_t& weight = 0) {}

    dist_t finding_shortest_path_with_saving(std::vector<vertex_t>& answer,
                                             const vertex_t& start,
                                             const vertex_t& end) {
        std::vector<dist_t> dist(vertexesNum_, kMaxValue_);
        std::vector<vertex_t> prev(vertexesNum_, 0);

        std::queue<vertex_t> tempQueue;
        tempQueue.push(start);
        
        dist[start] = 0;
        vertex_t currentVertex = 0;
        vertex_t newVertex = 0;
        int32_t step = 0;

        while (!tempQueue.empty()) {
            currentVertex = tempQueue.front();
            tempQueue.pop();
            step = 0;
            if (currentVertex == end) {
              break;
            }
            while (step < 4) {
                if (step == 0) {
                  newVertex = add_thousand(currentVertex);
                } else if (step == 1) {
                  newVertex = subtract_one(currentVertex);
                } else if (step == 2) {
                  newVertex = shift_right(currentVertex);
                } else {
                  newVertex = shift_left(currentVertex);
                }
                if (dist[newVertex] == kMaxValue_) {
                    dist[newVertex] = dist[currentVertex] + 1;
                    prev[newVertex] = currentVertex;
                    tempQueue.push(newVertex);
                }
                ++step;
            }
        }
        if (dist[end] == kMaxValue_) {
            return -1;
        }
        
        dist_t length = dist[end];
        vertex_t last = prev[end];
        answer.push_back(end);
        for (int32_t i = 0; i < length; ++i) {
            answer.push_back(last);
            last = prev[last];
        }

        return length + 1;
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
  const int32_t kMaxValue_ = INT_MAX;

  int32_t add_thousand(int32_t number) {
    return (number / 1000 == 9) ? number : number + 1000;
  }

  int32_t subtract_one(int32_t number) {
    return (number % 10 == 1) ? number : number - 1;
  }

  int32_t shift_right(int32_t number) {
    return (number % 10) * 1000 + number / 10;
  }

  int32_t shift_left(int32_t number) {
    return (number % 1000) * 10 + number / 1000;
  }
};

int main() {
  int32_t start = 0;
  int32_t end = 0;
  std::cin >> start >> end;
  GraphList graph(10001);

  std::vector<vertex_t> answer = {};
    dist_t lenth = graph.finding_shortest_path_with_saving(answer, start, end);

    std::cout << lenth << std::endl;
    for (int32_t i = 0; i < lenth; ++i) {
        std::cout << answer.back() << std::endl;
        answer.pop_back();
    }
    
    return 0;
}