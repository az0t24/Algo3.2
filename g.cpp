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

class GraphTable final : public Graph {
public:
    explicit GraphTable(size_t tableWide, size_t tableLenth) {
        tableWide_ = tableWide;
        tableLenth_ = tableLenth;

        cityMap_ = {};
        tubes_ = 0;

        std::vector<bool> falseLine(tableLenth_, false);
        used_.resize(tableWide_, falseLine);
    }

    void insert_edge(const vertex_t& first, const vertex_t& second,
                                          const weight_t& weight = 0) {}

    void add_new_line(std::vector<int32_t>& newLine) {
        cityMap_.push_back(newLine);
    }

    int32_t find_tubes() {
        int32_t answer = 0;

        for (int32_t coord_x = 0; coord_x < tableWide_; ++coord_x) {
            for (int32_t coord_y = 0; coord_y < tableLenth_; ++coord_y) {
                if (!used_[coord_x][coord_y]) {
                    tubes_++;
                    comps_.push_back(false);
                    pour_water(coord_x, coord_y);
                }
            }
        }

        for (int32_t i = 0; i < tubes_; ++i) {
            if (!comps_[i]) {
                ++answer;
            }
        }

        return answer;
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
    size_t tubes_;
    std::vector<std::vector<bool>> used_;
    std::vector<bool> comps_;
    std::vector<std::vector<int32_t>> cityMap_;
    const int32_t kMaxValue_ = INT_MAX;

    void pour_water(int32_t coord_x, int32_t coord_y) {
        if (used_[coord_x][coord_y]) {
            return;
        }
        used_[coord_x][coord_y] = true;
        if (check_board(coord_x - 1, coord_y)) {
            if (cityMap_[coord_x - 1][coord_y] == cityMap_[coord_x][coord_y]) {
                pour_water(coord_x - 1, coord_y);
            }
            if (cityMap_[coord_x - 1][coord_y] < cityMap_[coord_x][coord_y]) {
                comps_[tubes_ - 1] = true;
            }
        }

        if (check_board(coord_x + 1, coord_y)) {
            if (cityMap_[coord_x + 1][coord_y] == cityMap_[coord_x][coord_y]) {
                pour_water(coord_x + 1, coord_y);
            }
            if (cityMap_[coord_x + 1][coord_y] < cityMap_[coord_x][coord_y]) {
                comps_[tubes_ - 1] = true;
            }
        }

        if (check_board(coord_x, coord_y - 1)) {
            if (cityMap_[coord_x][coord_y - 1] == cityMap_[coord_x][coord_y]) {
                pour_water(coord_x, coord_y - 1);
            }
            if (cityMap_[coord_x][coord_y - 1] < cityMap_[coord_x][coord_y]) {
                comps_[tubes_ - 1] = true;
            }
        }

        if (check_board(coord_x, coord_y + 1)) {
            if (cityMap_[coord_x][coord_y + 1] == cityMap_[coord_x][coord_y]) {
                pour_water(coord_x, coord_y + 1);
            }
            if (cityMap_[coord_x][coord_y + 1] < cityMap_[coord_x][coord_y]) {
                comps_[tubes_ - 1] = true;
            }
        }
    }

    bool check_board(int32_t x, int32_t y) {
        return (x >= 0 && x < tableWide_ && y >= 0 && y < tableLenth_);
    }
};

int main() {
    int32_t tableWide = 0;
    int32_t tableLenth = 0;
    std::cin >> tableWide >> tableLenth;
    int32_t value = 0;
    GraphTable graph(tableWide, tableLenth);
    std::vector<int32_t> line(tableLenth);

    for (int32_t coord_x = 0; coord_x < tableWide; ++coord_x) {
        for (int32_t coord_y = 0; coord_y < tableLenth; ++coord_y) {
            std::cin >> value;
            line[coord_y] = value;
        }
        graph.add_new_line(line);
    }

    std::cout << graph.find_tubes() << std::endl;

    return 0;
}