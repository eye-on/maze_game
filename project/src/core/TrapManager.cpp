#include "../../include/core/TrapManager.hpp"
#include <cstdlib>
#include <ctime>

void TrapManager::generateTraps(int count, const Player& player, const Map& map) 
{
    traps.clear();
    std::srand(std::time(0)); // 随机种子

    int generated = 0;
    while (generated < count) 
    {
        // 随机生成地图范围内的位置
        int x = std::rand() % map.getWidth();
        int y = std::rand() % map.getHeight();

        // 检查：1.是通路 2.不在玩家位置 3.未重复生成
        bool isDuplicate = false;
        for (const auto& trap : traps) 
        {
            if (trap.getX() == x && trap.getY() == y) 
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate && map.isPositionValid(x, y) && !(x == player.getX() && y == player.getY())) 
        {
            traps.emplace_back(x, y);
            generated++;
        }
    }
}

void TrapManager::checkCollisions(Player& player) 
{
    for (auto& trap : traps) 
    {
        if (trap.isActive() && trap.getX() == player.getX() && trap.getY() == player.getY()) 
        {
            player.takeDamage(trap.trigger()); // 触发陷阱，玩家掉血
            break;
        }
    }
}