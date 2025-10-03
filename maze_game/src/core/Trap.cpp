#include "../../include/core/Trap.hpp"

Trap::Trap(int x, int y) : x(x), y(y), m_isActive(true) {}

int Trap::trigger() 
{
    if (m_isActive) 
    {
        m_isActive = false;
        return TRAP_DAMAGE; // 触发后返回伤害，陷阱失效
    }
    return 0;
}