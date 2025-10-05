#ifndef COMMON_HPP
#define COMMON_HPP

#include <SFML/Graphics.hpp>

// 通用枚举
enum class Direction { UP, DOWN, LEFT, RIGHT }; // 玩家移动方向
enum class MapElement { WALL, PATH, PLAYER, TRAP, UNKNOWN }; // 地图元素类型

// 游戏常量（可根据需要调整）
const int CELL_SIZE = 40;
const int BORDER_SIZE = 1;  // 边框单元格数量（上下左右各1格）
const int WINDOW_WIDTH = 800;  // 左右边框
const int WINDOW_HEIGHT = 600; // 上下边框
const int PLAYER_MAX_HP = 100;    // 玩家初始生命值
const int TRAP_DAMAGE = 20;       // 陷阱伤害
const int TRAP_COUNT = 5;         // 初始陷阱数量

// 颜色定义
const sf::Color COLOR_WALL = sf::Color(100, 100, 100);   // 墙（灰色）
const sf::Color COLOR_PATH = sf::Color::White;           // 通路（白色）
const sf::Color COLOR_PLAYER = sf::Color::Red;           // 玩家（红色）
const sf::Color COLOR_TRAP = sf::Color::Yellow;          // 陷阱（黄色）
const sf::Color COLOR_TEXT = sf::Color::White;           // 文字（白色）
const float TRAP_ACTIVE_TIME = 5.0f;    // 陷阱激活状态持续时间
const float TRAP_COOLDOWN_TIME = 3.0f;  // 陷阱冷却时间
#endif // COMMON_HPP