#ifndef TRAPMANAGER_H
#define TRAPMANAGER_H

#include "trap.hpp"
#include "player.hpp"
#include "map.hpp"
#include <vector>

class TrapManager {
private:
    std::vector<Trap> traps;  // 陷阱列表

public:
    TrapManager() = default;

    // 生成陷阱（确保不在玩家位置和墙上）
    void generateTraps(int count, const Player& player, const Map& map);

    // 检测玩家是否碰撞陷阱，处理伤害
    void checkCollisions(Player& player);

    // Getter
    const std::vector<Trap>& getTraps() const { return traps; }
};

#endif // TRAPMANAGER_H