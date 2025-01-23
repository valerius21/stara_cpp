#include "astar.hpp"

namespace {
    double heuristic(int x1, int y1, int x2, int y2) {
        return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    }

    struct NodeHash {
        std::size_t operator()(const std::shared_ptr<Node>& node) const {
            return std::hash<int>()(node->x) ^ (std::hash<int>()(node->y) << 1);
        }
    };

    struct NodeEqual {
        bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
            return *lhs == *rhs;
        }
    };
}

std::vector<std::pair<int, int>> findPath(
    const Maze& maze,
    std::pair<int, int> start,
    std::pair<int, int> goal
) {
    const std::vector<std::pair<int, int>> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0},  // Cardinal directions
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}  // Diagonal directions
    };

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, NodeCompare> openSet;
    std::unordered_set<std::shared_ptr<Node>, NodeHash, NodeEqual> closedSet;

    auto startNode = std::make_shared<Node>(start.first, start.second);
    startNode->h_cost = heuristic(start.first, start.second, goal.first, goal.second);
    openSet.push(startNode);

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        if (current->x == goal.first && current->y == goal.second) {
            // Path found, reconstruct it
            std::vector<std::pair<int, int>> path;
            auto node = current;
            while (node != nullptr) {
                path.emplace_back(node->x, node->y);
                node = node->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(current);

        for (const auto& dir : directions) {
            int newX = current->x + dir.first;
            int newY = current->y + dir.second;

            if (!maze.isWalkable(newX, newY)) {
                continue;
            }

            auto neighbor = std::make_shared<Node>(newX, newY);
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            double moveCost = (dir.first != 0 && dir.second != 0) ? 1.414 : 1.0;
            double tentative_g_cost = current->g_cost + moveCost;

            neighbor->g_cost = tentative_g_cost;
            neighbor->h_cost = heuristic(newX, newY, goal.first, goal.second);
            neighbor->parent = current;
            openSet.push(neighbor);
        }
    }

    return {};  // No path found
} 