#include "../../include/ui/Graphics.hpp"
#include <iostream>

// 为了在Menu中绘制矩形，添加draw重载（需要在Graphics.h中声明）
void Graphics::draw(const sf::Shape& shape) 
{
    window.draw(shape);
}

Graphics::Graphics() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Maze Game") 
{
    // 1. 强制禁用垂直同步（理由暂时未知）
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60); // 用帧率限制替代垂直同步
    
    // 2. 检查窗口是否成功创建
    if (!window.isOpen()) 
    {
        std::cerr << "致命错误：窗口创建失败！可能原因：\n";
        std::cerr << "- 不支持的分辨率：" << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << "\n";
        std::cerr << "- 显卡驱动问题或缺少显示权限\n";
        exit(EXIT_FAILURE); // 立即退出，避免后续操作无效窗口
    }

    //字体路径
    if (!font.loadFromFile("../assets/fonts/game_font.ttf")) 
    { 
        std::cerr << "警告：字体加载失败，文字无法显示\n";
    }
}
bool Graphics::pollEvent(sf::Event& event) 
{
    return window.pollEvent(event);
}

void Graphics::clear() 
{
    window.clear(sf::Color::Cyan); // 背景
}

void Graphics::display() 
{
    window.display();
}

void Graphics::drawMap(const Map& map) 
{
    for (int y = 0; y < map.getHeight(); y++) 
    {
        for (int x = 0; x < map.getWidth(); x++) 
        {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
            
            // 根据地图元素设置颜色
            if (!map.isExplored(x, y)) 
            {
                cell.setFillColor(sf::Color::Black);  // 未知区域为黑色
            } 
            else 
            {
                switch (map.getElement(x, y)) 
                {
                    case MapElement::WALL: cell.setFillColor(COLOR_WALL); break;
                    case MapElement::PATH: cell.setFillColor(COLOR_PATH); break;
                    default: break;
                }
            }

            // 绘制格子边框
            cell.setOutlineColor(sf::Color::Black);
            cell.setOutlineThickness(1);
            window.draw(cell);
        }
    }

    // 绘制终点（绿色方块）
    if (map.isExplored(map.getEndX(), map.getEndY())) 
    {
        sf::RectangleShape endMarker(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        endMarker.setPosition(map.getEndX() * CELL_SIZE, map.getEndY() * CELL_SIZE);
        endMarker.setFillColor(sf::Color::Green);
        window.draw(endMarker);
    }
}

void Graphics::drawPlayer(const Player& player) 
{
    sf::RectangleShape playerShape(sf::Vector2f(CELL_SIZE - 10, CELL_SIZE - 10));
    playerShape.setPosition(
        player.getX() * CELL_SIZE + 5,  // +5居中
        player.getY() * CELL_SIZE + 5
    );
    playerShape.setFillColor(COLOR_PLAYER);
    window.draw(playerShape);
}

void Graphics::drawTraps(const TrapManager& trapManager, const Map& map) 
{  
    for (const auto& trap : trapManager.getTraps()) 
    {
        if (trap.isActive() && map.isExplored(trap.getX(), trap.getY())) // 只显示已探索区域的陷阱
        {  
            sf::RectangleShape trapShape(sf::Vector2f(CELL_SIZE - 10, CELL_SIZE - 10));
            trapShape.setPosition(
                trap.getX() * CELL_SIZE + 5,
                trap.getY() * CELL_SIZE + 5
            );
            trapShape.setFillColor(COLOR_TRAP);
            window.draw(trapShape);
        }
    }
}

void Graphics::drawText(const std::string& text, int x, int y, int characterSize) 
{
    sf::Text sfText(text, font, characterSize);
    sfText.setFillColor(COLOR_TEXT);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

void Graphics::drawGameStatus(const Player& player, bool isWin) 
{
    // 显示生命值
    std::string hpText = "HP: " + std::to_string(player.getHP());
    drawText(hpText, 10, WINDOW_HEIGHT - 30);
}

void Graphics::drawMenuBackground() 
{
    // 绘制菜单背景（半透明白色）
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(sf::Color(255, 255, 255, 230));
    window.draw(background);
}