#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <memory>
#include <optional>

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

    std::vector<std::tuple<int, int, int>> getCellNeighbours(int x, int y) const {
        std::vector<std::tuple<int, int, int>> neighbours;
        const std::vector<std::pair<int, int>> directions = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0}  // Only cardinal directions like in Python version
        };

        for (const auto& dir : directions) {
            int newX = x + dir.first;
            int newY = y + dir.second;
            if (isValid(newX, newY)) {
                neighbours.emplace_back(newX, newY, grid_[newY][newX]);
            }
        }
        return neighbours;
    }

    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    std::vector<std::vector<int>> grid_;
    int width_;
    int height_;
};

class AStar {
public:
    // Hash function for pair
    struct PairHash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

    explicit AStar(const Maze& maze) : maze_(maze) {}

    static int manhattanDistance(const std::pair<int, int>& pos1, const std::pair<int, int>& pos2) {
        return std::abs(pos1.first - pos2.first) + std::abs(pos1.second - pos2.second);
    }

    static std::pair<int, int> getLowestFScoreNode(
        const std::unordered_map<std::pair<int, int>, int, PairHash>& fScores,
        const std::unordered_set<std::pair<int, int>, PairHash>& openSet
    );

    std::optional<std::vector<std::pair<int, int>>> findPath(
        const std::pair<int, int>& start,
        const std::pair<int, int>& goal
    );

private:
    const Maze& maze_;
}; 