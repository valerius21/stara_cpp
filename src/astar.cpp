#include "astar.hpp"

std::pair<int, int> AStar::getLowestFScoreNode(
    const std::unordered_map<std::pair<int, int>, int, PairHash>& fScores,
    const std::unordered_set<std::pair<int, int>, PairHash>& openSet
) {
    auto minIt = openSet.begin();
    int minScore = fScores.at(*minIt);

    for (auto it = std::next(openSet.begin()); it != openSet.end(); ++it) {
        int score = fScores.at(*it);
        if (score < minScore) {
            minScore = score;
            minIt = it;
        }
    }
    return *minIt;
}

std::optional<std::vector<std::pair<int, int>>> AStar::findPath(
    const std::pair<int, int>& start,
    const std::pair<int, int>& goal
) {
    // Track actual distance from start to each node
    std::unordered_map<std::pair<int, int>, int, PairHash> gScores;
    gScores[start] = 0;

    // Track estimated total distance through each node
    std::unordered_map<std::pair<int, int>, int, PairHash> fScores;
    fScores[start] = manhattanDistance(start, goal);

    // For path reconstruction
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, PairHash> cameFrom;

    // Set of nodes to evaluate
    std::unordered_set<std::pair<int, int>, PairHash> openSet;
    openSet.insert(start);

    // Keep track of visited nodes
    std::unordered_set<std::pair<int, int>, PairHash> closedSet;

    while (!openSet.empty()) {
        // Get node with lowest f_score
        auto currentPos = getLowestFScoreNode(fScores, openSet);
        openSet.erase(currentPos);

        if (closedSet.find(currentPos) != closedSet.end()) {
            continue;
        }

        // If we reached the goal, reconstruct and return the path
        if (currentPos == goal) {
            std::vector<std::pair<int, int>> path;
            auto current = currentPos;
            while (cameFrom.find(current) != cameFrom.end()) {
                path.push_back(current);
                current = cameFrom[current];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        closedSet.insert(currentPos);

        // Check all neighbors
        auto neighbors = maze_.getCellNeighbours(currentPos.first, currentPos.second);
        for (const auto& next : neighbors) {
            auto nextPos = std::make_pair(std::get<0>(next), std::get<1>(next));
            int value = std::get<2>(next);

            // Skip walls and already processed nodes
            if (value == 0 || closedSet.find(nextPos) != closedSet.end()) {
                continue;
            }

            // Calculate tentative g_score for this neighbor
            // All edges have weight 1 in this implementation
            int tentativeG = gScores[currentPos] + 1;

            // If we found a better path to this neighbor
            if (gScores.find(nextPos) == gScores.end() || tentativeG < gScores[nextPos]) {
                // Update the path
                cameFrom[nextPos] = currentPos;
                gScores[nextPos] = tentativeG;
                fScores[nextPos] = tentativeG + manhattanDistance(nextPos, goal);
                openSet.insert(nextPos);
            }
        }
    }

    // If we get here, no path exists
    return std::nullopt;
} 