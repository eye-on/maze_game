#ifndef TRAPMANAGER_HPP
#define TRAPMANAGER_HPP

#include "Trap.hpp"
#include "Player.hpp"
#include "Map.hpp"
#include <vector>

class TrapManager 
{
private:
    std::vector<Trap> traps;  // 陷阱列表
    bool findValidPosition(int& x, int& y, const Player& player, const Map& map);
public:
    TrapManager() = default;

    // 生成陷阱（确保不在玩家位置和墙上）
    void generateTraps(int count, const Player& player, const Map& map);

    // 检测玩家是否碰撞陷阱，处理伤害
    void checkCollisions(Player& player);

     // 更新所有陷阱状态（每帧调用）
    void update(float deltaTime, const Player& player, const Map& map);
    
    // Getter
    const std::vector<Trap>& getTraps() const { return traps; }
};

#endif // TRAPMANAGER_HPP