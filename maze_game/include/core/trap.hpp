#ifndef TRAP_H
#define TRAP_H

#include "../utils/common.hpp"

class Trap {
private:
    int x, y;          // 陷阱在地图中的格子坐标
    bool isActive;     // 是否激活（未被触发）

public:
    Trap(int x, int y);

    // 触发陷阱（返回伤害值，之后陷阱失效）
    int trigger();

    // Getter
    int getX() const { return x; }
    int getY() const { return y; }
    bool isActive() const { return isActive; }
};

#endif // TRAP_H