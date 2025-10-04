#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "../utils/Common.hpp"
#include "../core/Player.hpp"
#include "../core/Map.hpp"
#include "../core/TrapManager.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class Graphics 
{
private:
    sf::RenderWindow window;  // 渲染窗口
    sf::Font font;            
    
public:
    Graphics();
    void draw(const sf::Shape& shape);
    // 窗口是否打开
    bool isWindowOpen() const { return window.isOpen(); }

    // 处理窗口事件（如关闭）
    bool pollEvent(sf::Event& event);

    // 清屏
    void clear();

    // 显示
    void display();

    // 绘制地图
    void drawMap(const Map& map);

    // 绘制玩家
    void drawPlayer(const Player& player);

    // 绘制陷阱
    void drawTraps(const TrapManager& trapManager, const Map& map);

    // 鞋子
    void drawText(const std::string& text, int x, int y, int characterSize = 24);

    // 绘制游戏状态（生命值、胜利/失败提示）
    void drawGameStatus(const Player& player, bool isWin);

    // 绘制菜单背景和边框
    void drawMenuBackground();

    sf::RenderWindow& getWindow() { return window; }
};

#endif // GRAPHICS_HPP