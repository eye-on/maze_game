#include "../../include/ui/Menu.hpp"
#include "../../include/ui/Graphics.hpp"
#include <SFML/Window/Keyboard.hpp>

Menu::Menu() : options({"Start Game", "Exit", "Map 1", "Map 2"}), selectedIndex(0) {}

int Menu::handleInput() 
{
    // 菜单未激活时不处理任何输入
    if (!isActive) 
    {
        return -1; 
    }
    // 上下键切换选项
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && selectedIndex > 0) 
    {
        selectedIndex--;
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)); // 等待释放
        return -1;
    } 
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && selectedIndex < options.size() - 1) 
    {
        selectedIndex++;
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)); // 等待释放
        return -1;
    } 
    // 处理Enter键（核心修复）
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) 
    {
        // 如果是刚进入菜单，忽略第一次Enter
        if (ignoreFirstEnter) 
        {
            // 等待Enter释放后，才允许后续输入
            while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return));
            ignoreFirstEnter = false;
            return -1;
        }
        // 正常情况：等待释放后返回选中项
        while (sf::Keyboard::isKeyPressed(sf::Keyboard::Return));
        return selectedIndex;
    }
    return -1;
}
//设置菜单激活状态
void Menu::setActive(bool active) 
{
    isActive = active;
    if (active) 
    {
        // 刚激活时需要忽略一次Enter，防止状态切换带来的残留输入
        ignoreFirstEnter = true;
    }
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