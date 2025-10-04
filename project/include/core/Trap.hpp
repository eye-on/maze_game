#ifndef TRAP_HPP
#define TRAP_HPP

#include "../utils/Common.hpp"

class Trap 
{
private:
    int x, y;          // 陷阱在地图中的格子坐标
    bool m_isActive;     // 是否激活（未被触发）

public:
    Trap(int x, int y);

    // 触发陷阱（返回伤害值，之后陷阱失效）
    int trigger();

    // Getter
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return m_isActive; }
};

#endif // TRAP_HPP