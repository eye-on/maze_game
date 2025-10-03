#ifndef COMMON_H
#define COMMON_H

#include <SFML/graphics.hpp>

// 通用枚举
enum class Direction { UP, DOWN, LEFT, RIGHT }; // 玩家移动方向
enum class MapElement { WALL, PATH, PLAYER, TRAP }; // 地图元素类型

// 游戏常量（可根据需要调整）
const int CELL_SIZE = 40;         // 每个格子的像素大小
const int MAP_WIDTH = 15;         // 地图宽度（格子数）
const int MAP_HEIGHT = 10;        // 地图高度（格子数）
const int WINDOW_WIDTH = MAP_WIDTH * CELL_SIZE;  // 窗口宽度
const int WINDOW_HEIGHT = MAP_HEIGHT * CELL_SIZE; // 窗口高度
const int PLAYER_MAX_HP = 100;    // 玩家初始生命值
const int TRAP_DAMAGE = 20;       // 陷阱伤害
const int TRAP_COUNT = 5;         // 初始陷阱数量

// 颜色定义
const sf::Color COLOR_WALL = sf::Color(100, 100, 100);   // 墙（灰色）
const sf::Color COLOR_PATH = sf::Color::White;           // 通路（白色）
const sf::Color COLOR_PLAYER = sf::Color::Red;           // 玩家（红色）
const sf::Color COLOR_TRAP = sf::Color::Yellow;          // 陷阱（黄色）
const sf::Color COLOR_TEXT = sf::Color::Black;           // 文字（黑色）

#endif // COMMON_H
