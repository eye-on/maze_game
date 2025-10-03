#ifndef PLAYER_H
#define PLAYER_H

#include "../utils/common.hpp"

class Player {
private:
    int x, y;          // 玩家在地图中的格子坐标（x:列, y:行）
    int hp;            // 生命值
    bool isAlive;      // 是否存活

public:
    // 构造函数：初始位置(x,y)，默认生命值
    Player(int startX, int startY);

    // 移动方法：返回是否移动成功（不穿墙/不出界）
    bool move(Direction dir, const class Map& map);

    // 受伤（减少生命值）
    void takeDamage(int damage);

    // Getter
    int getX() const { return x; }
    int getY() const { return y; }
    int getHP() const { return hp; }
    bool isAlive() const { return isAlive; }
};

#endif // PLAYER_H