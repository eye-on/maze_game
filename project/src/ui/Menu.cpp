#include "../../include/ui/Menu.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
Menu::Menu() 
{
    // 加载字体（请确保字体文件路径正确，若无字体可注释此部分）
    if (!font.loadFromFile("assets/fonts/game_font.ttf")) 
    {
        // 字体加载失败时使用系统默认（不影响功能，文字可能显示为方块）
        std::cerr << "警告：未找到字体文件，使用默认字体" << std::endl;
    }

    // 预计算按钮位置（居中显示）
    float startY = (WINDOW_HEIGHT - (BTN_HEIGHT * 2 + BTN_SPACING)) / 2;
    for (int i = 0; i < options.size(); ++i) 
    {
        float x = (WINDOW_WIDTH - BTN_WIDTH) / 2;
        float y = startY + i * (BTN_HEIGHT + BTN_SPACING);
        btnBounds.emplace_back(x, y, BTN_WIDTH, BTN_HEIGHT);
    }
}
void Menu::handleEvent(const sf::Event& event) 
{
    // 处理键盘按键按下事件
    if (event.type == sf::Event::KeyPressed) 
    {
        if (event.key.code == sf::Keyboard::Up) 
        {
            upPressed = true;
        } else if (event.key.code == sf::Keyboard::Down) 
        {
            downPressed = true;
        } else if (event.key.code == sf::Keyboard::Return) 
        {
            returnPressed = true;
        }
    }
    // 处理鼠标左键按下事件
    else if (event.type == sf::Event::MouseButtonPressed) 
    {
        if (event.mouseButton.button == sf::Mouse::Left) 
        {
            leftMousePressed = true;
        }
    }
}
int Menu::handleInput(const sf::RenderWindow& window) 
{  
    if (!isActive) {
        return -1;
    }

    // 首次输入忽略（基于事件标志判断是否有输入）
    bool hasInput = upPressed || downPressed || returnPressed || leftMousePressed;
    if (ignoreFirstInput && hasInput) 
    {
        ignoreFirstInput = false;
        // 重置所有事件标志
        upPressed = downPressed = returnPressed = leftMousePressed = false;
        return -1;
    }

    // 1. 键盘控制（基于事件标志）
    if (upPressed && selectedIndex > 0) 
    {
        selectedIndex--;
        upPressed = false; // 处理后重置标志
    } else if (downPressed && selectedIndex < 1) 
    {
        selectedIndex++;
        downPressed = false; // 处理后重置标志
    }
    if (returnPressed) 
    {
        returnPressed = false; // 处理后重置标志
        return selectedIndex;
    }

    // 2. 鼠标控制（基于事件标志）
    sf::Vector2f mousePos = static_cast<sf::Vector2f>(
        sf::Mouse::getPosition(window)
    );
    if (leftMousePressed) 
    {
        for (int i = 0; i < btnBounds.size(); ++i) 
        {
            if (btnBounds[i].contains(mousePos)) 
            {
                selectedIndex = i;
                leftMousePressed = false; // 处理后重置标志
                return selectedIndex;
            }
        }
        leftMousePressed = false; // 点击非按钮区域也重置标志
    }
    return -1;
}
void Menu::setActive(bool active) 
{
    isActive = active;
    if (active) 
    {
        // 刚激活时忽略一次输入（防止状态切换的残留按键）
        ignoreFirstInput = true;
    }
}

void Menu::draw(sf::RenderWindow& window) const
{
    // 绘制标题（可选，增强视觉效果）
    sf::Text title("labyrinth", font, 48);
    title.setFillColor(sf::Color::Black);
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setPosition(
        (WINDOW_WIDTH - titleRect.width) / 2,
        btnBounds[0].top - 100  // 标题在按钮上方100像素
    );
    window.draw(title);

    // 绘制按钮
    for (int i = 0; i < options.size(); ++i) 
    {
        // 按钮背景
        sf::RectangleShape btn(sf::Vector2f(BTN_WIDTH, BTN_HEIGHT));
        btn.setPosition(btnBounds[i].left, btnBounds[i].top);
        btn.setFillColor(i == selectedIndex ? BTN_HOVER : BTN_NORMAL);
        btn.setOutlineColor(BTN_BORDER);
        btn.setOutlineThickness(3);  // 选中时边框更明显
        if (i == selectedIndex) 
        {
            btn.setScale(1.05f, 1.05f);  // 选中时轻微放大
            btn.setPosition(
                btnBounds[i].left - (BTN_WIDTH * 0.05f)/2,
                btnBounds[i].top - (BTN_HEIGHT * 0.05f)/2
            );
        }
        window.draw(btn);

        // 按钮文字
        sf::Text text(options[i], font, TEXT_SIZE);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setPosition(
            btnBounds[i].left + (BTN_WIDTH - textRect.width)/2,
            btnBounds[i].top + (BTN_HEIGHT - textRect.height)/2 - 5
        );
        text.setFillColor(TEXT_COLOR);
        window.draw(text);
    }
}