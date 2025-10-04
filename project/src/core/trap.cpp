#include "../../include/core/trap.hpp"

Trap::Trap(int x, int y) : x(x), y(y), isActive(true) {}

int Trap::trigger() {
    if (isActive) {
        isActive = false;
        return TRAP_DAMAGE; // 触发后返回伤害，陷阱失效
    }
    return 0;
}