#ifndef GAME_H
#define GAME_H

#include "player.hpp"
#include "map.hpp"
#include "trapmanager.hpp"
#include "../ui/menu.hpp"
#include "../ui/graphics.hpp"

// 游戏状态
enum class GameState { MENU, PLAYING, GAME_OVER, WIN };

class Game {
private:
    GameState state;          // 当前游戏状态
    Player player;            // 玩家
    Map map;                  // 地图
    TrapManager trapManager;  // 陷阱管理器
    Menu menu;                // 菜单
    Graphics graphics;        // 图形渲染
    int currentMapIndex;      // 当前选中的地图索引（0=map1,1=map2）

public:
    // 构造函数：初始化游戏
    Game();

    // 启动游戏主循环
    void run();

private:
    // 处理输入（菜单/游戏中）
    void handleInput();

    // 更新游戏状态（移动、碰撞检测等）
    void update();

    // 渲染画面
    void render();

    // 重置游戏（重新加载地图、玩家、陷阱）
    void resetGame();
};

#endif // GAME_H