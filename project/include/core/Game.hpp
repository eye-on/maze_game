#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include "Map.hpp"
#include "TrapManager.hpp"
#include "../ui/Menu.hpp"
#include "../ui/Graphics.hpp"
#include <SFML/System/Clock.hpp> 
// 游戏状态
enum class GameState { MENU, PLAYING, GAME_OVER, WIN };

class Game 
{
private:
    GameState state;          // 游戏状态
    Player player;            // 玩家
    Map map;                  // 地图
    TrapManager trapManager;  // 陷阱管理
    Menu menu;                // 菜单
    Graphics graphics;        // 图形渲染
    int currentMapIndex;      // 当前选中的地图索引（0=map1,1=map2）
    bool wPressed = false;    // 想必人类都能看懂吧
    bool sPressed = false;
    bool aPressed = false;
    bool dPressed = false;
    sf::Clock gameClock;
public:
    // 构造函数：初始化
    Game();

    // 启动!
    void run();

private:
    // 处理输入（菜单/游戏中）
    void handleInput();

    // 更新状态
    void update();

    // 渲染画面
    void render();

    // 重置游戏
    void resetGame();
};

#endif // GAME_HPP