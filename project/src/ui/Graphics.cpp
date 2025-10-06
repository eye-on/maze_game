#include "../../include/ui/Graphics.hpp"
#include <iostream>

std::pair<int, int> Graphics::getMapOffset(const Map& map) const {
    int mapWidth = map.getWidth() * cellSize;
    int mapHeight = map.getHeight() * cellSize;
    int offsetX = (WINDOW_WIDTH - mapWidth) / 2;
    int offsetY = (WINDOW_HEIGHT - mapHeight) / 2;
    return {offsetX, offsetY};
}
// 为了在Menu中绘制矩形，添加draw重载（需要在Graphics.h中声明）
void Graphics::draw(const sf::Shape& shape) 
{
    window.draw(shape);
}

Graphics::Graphics() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Maze Game"),cellSize(40) 
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
    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "迷宫游戏");
    //字体路径
    if (!font.loadFromFile("../assets/fonts/game_font.ttf")) 
    { 
        std::cerr << "警告：字体加载失败，文字无法显示\n";
    }
}
void Graphics::calculateCellSize(const Map& map) 
{
   if (map.getWidth() == 0 || map.getHeight() == 0) {
       cellSize = 40;  // 默认值
       return;
   }

   // 计算基于窗口宽度和地图宽度的最大可能格子大小
   int maxWidthBased = WINDOW_WIDTH / map.getWidth();
   // 计算基于窗口高度和地图高度的最大可能格子大小
   int maxHeightBased = WINDOW_HEIGHT / map.getHeight();

   // 取较小值作为格子大小（确保地图能完整显示）
   cellSize = std::min(maxWidthBased, maxHeightBased);

   // 限制最小格子大小（避免过小无法显示）
   cellSize = std::max(cellSize, 10);  // 最小10像素
}
bool Graphics::pollEvent(sf::Event& event) 
{
    return window.pollEvent(event);  
}

void Graphics::clear(const sf::Color& color) 
{
    window.clear(color);  
}

void Graphics::display() 
{
    window.display();
}

void Graphics::drawMap(const Map& map) 
{
    int mapWidth = map.getWidth() * cellSize;
    int mapHeight = map.getHeight() * cellSize;
    int offsetX = (WINDOW_WIDTH - mapWidth) / 2;
    int offsetY = (WINDOW_HEIGHT - mapHeight) / 2;
    
    for (int y = 0; y < map.getHeight(); y++) 
    {
        for (int x = 0; x < map.getWidth(); x++) 
        {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(x * cellSize + offsetX, y * cellSize + offsetY);
            
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
            cell.setOutlineColor(sf::Color::White);
            cell.setOutlineThickness(1);
            window.draw(cell);
        }
    }

    // 绘制终点（绿色方块）
    if (map.isExplored(map.getEndX(), map.getEndY())) 
    {
        sf::RectangleShape endMarker(sf::Vector2f(cellSize, cellSize));
        // 关键修正：终点位置 = 原有坐标 + 偏移量
        endMarker.setPosition(
            map.getEndX() * cellSize + offsetX,  // 加上X方向偏移
            map.getEndY() * cellSize + offsetY   // 加上Y方向偏移
        );
        endMarker.setFillColor(sf::Color::Green);
        window.draw(endMarker);
    }
}

void Graphics::drawPlayer(const Player& player, const Map& map) 
{  
    auto [offsetX, offsetY] = getMapOffset(map);  // 统一偏移
    
    sf::RectangleShape playerShape(sf::Vector2f(cellSize - int(cellSize/4), cellSize - int(cellSize/4)));
    playerShape.setPosition(
        player.getX() * cellSize + offsetX + int(cellSize/8),  // 基于地图居中偏移
        player.getY() * cellSize + offsetY + int(cellSize/8)
    );
    playerShape.setFillColor(COLOR_PLAYER);
    window.draw(playerShape);
}

void Graphics::drawTraps(const TrapManager& trapManager, const Map& map) 
{
    auto [offsetX, offsetY] = getMapOffset(map);  // 统一偏移
    
    for (const auto& trap : trapManager.getTraps()) 
    {
        if (trap.isActive() && map.isExplored(trap.getX(), trap.getY())) 
        {
            sf::RectangleShape trapShape(sf::Vector2f(cellSize - int(cellSize/4), cellSize - int(cellSize/4)));
            trapShape.setPosition(
                trap.getX() * cellSize + offsetX + int(cellSize/8),  // 基于地图居中偏移
                trap.getY() * cellSize + offsetY + int(cellSize/8)
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

void Graphics::drawGameStatus(const Player& player, const Map& map, bool isGameOver) 
{
    const int marginX = 20;   // 左边缘距离
    const int marginY = 20;   // 下边缘距离（从窗口底部往上20像素）
    std::string hpText = "HP: " + std::to_string(player.getHP());
    
    // 计算底部Y坐标：窗口高度 - 文字高度 - 下边缘距
    
    int statusY = WINDOW_HEIGHT - 30 - marginY;
    
    // 绘制血量（底部左侧）
    drawText(
        hpText, 
        marginX,    // 左边距20
        statusY     // 底部上移20
    );

    // 游戏结束文字：底部居中
    if (isGameOver) 
    {
        sf::Text gameOverText("Game Over!", font, 24);
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        drawText(
            "Game Over!",
            (WINDOW_WIDTH - textRect.width) / 2,  // 水平居中
            statusY                               // 与血量同高度
        );
    }
}

void Graphics::drawMenuBackground() 
{
    // 绘制菜单背景（半透明白色）
    sf::RectangleShape background(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setFillColor(sf::Color(255, 255, 255, 230));
    window.draw(background);
}