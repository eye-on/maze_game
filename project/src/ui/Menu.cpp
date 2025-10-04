#include "../../include/ui/Menu.hpp"
#include "../../include/ui/Graphics.hpp"
#include <SFML/Window/Keyboard.hpp>

Menu::Menu() : options({"Start Game", "Exit", "Map 1", "Map 2"}), selectedIndex(0) {}

int Menu::handleInput() 
{
    // 上下键切换选项
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && selectedIndex > 0) 
    {
        selectedIndex--;
        // 等待按键释放，避免快速切换
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up));
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && selectedIndex < options.size() - 1) 
    {
        selectedIndex++;
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down));
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) 
    {
        return selectedIndex; // 回车返回选中的选项索引
    }
    return -1; // 未选择
}

void Menu::draw(Graphics& graphics) 
{
    // 绘制标题
    graphics.drawText("Maze Game", WINDOW_WIDTH/2 - 100, 50, 36);

    // 绘制选项（选中项高亮）
    for (size_t i = 0; i < options.size(); i++) 
    {
        int yPos = 150 + i * 50;
        if (i == selectedIndex) 
        {
            // 选中项绘制背景
            sf::RectangleShape highlight(sf::Vector2f(200, 30));
            highlight.setPosition(WINDOW_WIDTH/2 - 100, yPos - 5);
            highlight.setFillColor(sf::Color(200, 200, 200));
            graphics.draw(highlight); // 需要在Graphics中添加draw方法重载
        }
        graphics.drawText(options[i], WINDOW_WIDTH/2 - 80, yPos);
    }
}