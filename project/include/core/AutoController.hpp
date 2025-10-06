#ifndef AUTOCONTROLLER_HPP
#define AUTOCONTROLLER_HPP

#include "Player.hpp"
#include "Map.hpp"
#include "TrapManager.hpp"
#include "../utils/Common.hpp"
#include <queue>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <memory>

// A*算法节点
struct Node {
    int x, y;
    float g_cost;  // 起点到当前节点的代价
    float h_cost;  // 当前节点到终点的预估代价
    std::shared_ptr<Node> parent;

    Node(int x_, int y_) : x(x_), y(y_), g_cost(0), h_cost(0), parent(nullptr) {}
    float f_cost() const { return g_cost + h_cost; } // 总代价
};

// 哈希函数用于unordered_map
struct PairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        return p.first * 1000 + p.second; // 简单哈希
    }
};

class AutoController {
private:
    // 启发函数（曼哈顿距离）
    float heuristic(int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }
    //回头惩罚
    const float BACKTRACK_PENALTY = 1.5f;
    // 检查位置是否安全（合法且无激活陷阱）
    bool isSafe(int x, int y, const Map& map, const TrapManager& trapManager) {
        if (!map.isPositionValid(x, y)) return false; // 检查是否是通路
        for (const auto& trap : trapManager.getTraps()) {
            if (trap.isActive() && trap.getX() == x && trap.getY() == y) {
                return false; // 避开激活的陷阱
            }
        }
        return true;
    }

    // 回溯路径
    std::vector<Direction> reconstructPath(std::shared_ptr<Node> endNode) {
        std::vector<Direction> path;
        auto current = endNode;
        while (current->parent) {
            int dx = current->x - current->parent->x;
            int dy = current->y - current->parent->y;

            if (dx == 1) path.push_back(Direction::RIGHT);
            else if (dx == -1) path.push_back(Direction::LEFT);
            else if (dy == 1) path.push_back(Direction::DOWN);
            else if (dy == -1) path.push_back(Direction::UP);

            current = current->parent;
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

public:
    // 决策下一步移动方向
    Direction decideNextMove(const Player& player, const Map& map, const TrapManager& trapManager) {
        int startX = player.getX();
        int startY = player.getY();
        int targetX = map.getEndX();
        int targetY = map.getEndY();

        // A*算法寻找路径
        auto cmp = [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
            return a->f_cost() > b->f_cost(); // 小顶堆
        };
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> openSet(cmp);
        std::unordered_map<std::pair<int, int>, std::shared_ptr<Node>, PairHash> closedSet;

        auto startNode = std::make_shared<Node>(startX, startY);
        startNode->h_cost = heuristic(startX, startY, targetX, targetY);
        openSet.push(startNode);

        // 四个方向（上下左右）
        std::vector<std::pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

        while (!openSet.empty()) {
            auto currentNode = openSet.top();
            openSet.pop();

            // 到达终点
            if (currentNode->x == targetX && currentNode->y == targetY) 
            {
                auto path = reconstructPath(currentNode);
                return path.empty() ? Direction::UP : path[0];
            }

            closedSet[{currentNode->x, currentNode->y}] = currentNode;

            // 探索邻居
            for (const auto& dir : directions) {
                int newX = currentNode->x + dir.first;
                int newY = currentNode->y + dir.second;
                float moveCost = 1.0f;
                if (!isSafe(newX, newY, map, trapManager)) continue;
                if (closedSet.count({newX, newY})) continue;
                bool isBacktrack = closedSet.count({newX, newY}) && 
                      !(currentNode->parent && 
                        currentNode->parent->x == newX && 
                        currentNode->parent->y == newY);
                if (isBacktrack) 
                {
                    moveCost *= BACKTRACK_PENALTY;  // 乘以惩罚系数
                }
                float newG = currentNode->g_cost + moveCost;
                std::shared_ptr<Node> neighborNode = nullptr;

                // 检查是否在openSet中
                bool found = false;
                auto tempQueue = openSet;
                while (!tempQueue.empty()) 
                {
                    auto node = tempQueue.top();
                    tempQueue.pop();
                    if (node->x == newX && node->y == newY) 
                    {
                        neighborNode = node;
                        found = true;
                        break;
                    }
                }

                if (!found) 
                {
                    neighborNode = std::make_shared<Node>(newX, newY);
                    neighborNode->parent = currentNode;
                    neighborNode->g_cost = newG;
                    neighborNode->h_cost = heuristic(newX, newY, targetX, targetY);
                    openSet.push(neighborNode);
                } 
                else if (newG < neighborNode->g_cost) 
                {
                    neighborNode->parent = currentNode;
                    neighborNode->g_cost = newG;
                }
            }
        }

        // 找不到路径时随机移动（避开陷阱）
        std::vector<Direction> validDirs;
        for (const auto& dir : directions) {
            int newX = startX + dir.first;
            int newY = startY + dir.second;
            if (isSafe(newX, newY, map, trapManager)) {
                if (dir.first == 1) validDirs.push_back(Direction::RIGHT);
                else if (dir.first == -1) validDirs.push_back(Direction::LEFT);
                else if (dir.second == 1) validDirs.push_back(Direction::DOWN);
                else if (dir.second == -1) validDirs.push_back(Direction::UP);
            }
        }
        return validDirs.empty() ? Direction::UP : validDirs[rand() % validDirs.size()];
    }
};

#endif // AUTOCONTROLLER_HPP