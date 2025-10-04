#include "../../include/core/TrapManager.hpp"
#include <cstdlib>
#include <ctime>

void TrapManager::generateTraps(int count, const Player& player, const Map& map) 
{
    traps.clear();
    std::srand(std::time(0));

    // 初始生成指定数量的陷阱
    for (int i = 0; i < count; ++i) 
    {
        int x, y;
        if (findValidPosition(x, y, player, map)) 
        {
            traps.emplace_back(x, y);
        }
    }
}

void TrapManager::checkCollisions(Player& player) 
{
    for (auto& trap : traps) 
    {
        if (trap.isActive() && trap.getX() == player.getX() && trap.getY() == player.getY()) 
        {
            player.takeDamage(trap.trigger());
            break;
        }
    }
}
void TrapManager::update(float deltaTime, const Player& player, const Map& map) 
{
    for (auto& trap : traps) 
    {
        // 更新陷阱状态（返回是否需要处理重生）
        if (trap.update(deltaTime)) 
        {
            // 如果陷阱需要重生，寻找新位置
            if (trap.needsRespawn()) 
            {
                int newX, newY;
                if (findValidPosition(newX, newY, player, map)) 
                {
                    trap.reset(newX, newY);
                }
            }
        }
    }
}

// 寻找合法的陷阱位置（不在玩家脚下、是通路、不与其他激活陷阱重叠）
bool TrapManager::findValidPosition(int& x, int& y, const Player& player, const Map& map) 
{
    int attempts = 0;
    const int MAX_ATTEMPTS = 100;  // 防止无限循环

    while (attempts < MAX_ATTEMPTS) 
    {
        // 随机生成地图范围内的位置
        x = std::rand() % map.getWidth();
        y = std::rand() % map.getHeight();

        // 检查1：是通路
        if (!map.isPositionValid(x, y)) 
        {
            attempts++;
            continue;
        }

        // 检查2：不在玩家位置
        if (x == player.getX() && y == player.getY()) 
        {
            attempts++;
            continue;
        }

        // 检查3：不与其他激活陷阱重叠
        bool isDuplicate = false;
        for (const auto& other : traps) 
        {
            if (other.isActive() && other.getX() == x && other.getY() == y) 
            {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) 
        {
            return true;  // 找到合法位置
        }

        attempts++;
    }

    return false;  // 多次尝试后仍未找到位置（地图太挤）
}