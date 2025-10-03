#include "../../include/core/player.hpp"
#include "../../include/core/map.hpp"

Player::Player(int startX, int startY) 
    : x(startX), y(startY), hp(PLAYER_MAX_HP), isAlive(true) {}

bool Player::move(Direction dir, const Map& map) {
    int newX = x;
    int newY = y;

    // 计算新位置
    switch (dir) {
        case Direction::UP:    newY--; break;
        case Direction::DOWN:  newY++; break;
        case Direction::LEFT:  newX--; break;
        case Direction::RIGHT: newX++; break;
    }

    // 检查新位置是否合法（在地图内且是通路）
    if (map.isPositionValid(newX, newY)) {
        x = newX;
        y = newY;
        return true;
    }
    return false;
}

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        isAlive = false;
    }
}