#include "../../include/core/Trap.hpp"

Trap::Trap(int x, int y) 
    : x(x), y(y), m_isActive(true), 
      activeTimer(0.0f), cooldownTimer(0.0f), 
      shouldRespawn(true) {}

int Trap::trigger() 
{
    if (m_isActive) 
    {
        m_isActive = false;
        shouldRespawn = false;  // 被触发的陷阱不会重生
        return TRAP_DAMAGE;
    }
    return 0;
}

// 更新陷阱状态：处理自动消失和冷却
bool Trap::update(float deltaTime) 
{
    if (m_isActive) 
    {
        // 激活状态：计时到期后自动消失
        activeTimer += deltaTime;
        if (activeTimer >= TRAP_ACTIVE_TIME) 
        {
            m_isActive = false;
            activeTimer = 0.0f;
            return true;  // 通知管理器需要进入冷却
        }
    }
    else if (shouldRespawn) 
    {
        // 冷却状态：计时到期后需要重新生成
        cooldownTimer += deltaTime;
        if (cooldownTimer >= TRAP_COOLDOWN_TIME) 
        {
            cooldownTimer = 0.0f;
            return true;  // 通知管理器可以重新生成
        }
    }
    return false;
}

// 重置陷阱到新位置（用于重新生成）
void Trap::reset(int newX, int newY) 
{
    x = newX;
    y = newY;
    m_isActive = true;
    activeTimer = 0.0f;
    shouldRespawn = true;
}