#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <memory>

class Maze {
public:
    Maze(const std::vector<std::vector<int>>& grid) : grid_(grid) {
        height_ = grid.size();
        width_ = height_ > 0 ? grid[0].size() : 0;
    }

    bool isValid(int x, int y) const {
        return x >= 0 && x < width_ && y >= 0 && y < height_;
    }

    bool isWalkable(int x, int y) const {
        return isValid(x, y) && grid_[y][x] >= 1;
    }

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    std::vector<std::vector<int>> grid_;
    int width_;
    int height_;
};

struct Node {
    int x, y;
    double g_cost;  // Cost from start to current node
    double h_cost;  // Heuristic cost from current node to goal
    std::shared_ptr<Node> parent;

    Node(int x, int y) : x(x), y(y), g_cost(0), h_cost(0), parent(nullptr) {}

    double f_cost() const { return g_cost + h_cost; }

    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
};

struct NodeCompare {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return a->f_cost() > b->f_cost();
    }
};

std::vector<std::pair<int, int>> findPath(
    const Maze& maze,
    std::pair<int, int> start,
    std::pair<int, int> goal
); 