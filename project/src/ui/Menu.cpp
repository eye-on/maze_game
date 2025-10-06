#include "../../include/ui/Menu.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
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
    for (int i = 0; i < mainOptions.size(); ++i) 
    {
        float x = (WINDOW_WIDTH - BTN_WIDTH) / 2;
        float y = startY + i * (BTN_HEIGHT + BTN_SPACING);
        btnBounds.emplace_back(x, y, BTN_WIDTH, BTN_HEIGHT);
    }
}
void Menu::loadMapList(const std::string& directory) 
{
    mapOptions.clear();
    try 
    {
        for (const auto& entry : fs::directory_iterator(directory)) 
        {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") 
            {
                mapOptions.push_back(entry.path().filename().string());
            }
        }
    } 
    catch (const fs::filesystem_error& e) 
    {
        std::cerr << "Error loading maps: " << e.what() << std::endl;
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
    int maxIndex = 2;
    if (!isActive) 
    {
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
    if (currentState == MenuState::MAIN) 
    {
        // 处理主菜单输入
        if (returnPressed) 
        {
            returnPressed = false;
            if (selectedIndex == 0) 
            {
                // 原"Start Game"逻辑（进入地图选择）
                currentState = MenuState::MAP_SELECT;
                selectedIndex = 0;
            } 
            else if (selectedIndex == 1) 
            {
                // 新按钮"Image to Map"：返回标识2
                return 2; 
            }
            else if (selectedIndex == 2) 
            {
                // 原"Exit"逻辑
                return 1;
            }
            return selectedIndex;
        }
    } 
    else if (currentState == MenuState::MAP_SELECT) 
    {
        // 处理地图选择输入
        if (returnPressed) 
        {
            returnPressed = false;
            currentState = MenuState::MAIN;  // 回到主菜单
            return 100 + selectedIndex;  // 返回地图选择结果（用100偏移表示）
        } 
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
        {
            currentState = MenuState::MAIN;
            selectedIndex = 0;
        }
    }
    // 1. 键盘控制（基于事件标志）
    if (upPressed && selectedIndex > 0) 
    {
        selectedIndex--;
        upPressed = false; // 处理后重置标志
    } 
    else if (downPressed && selectedIndex < maxIndex) 
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
    if (!isActive) return; // 菜单未激活时不绘制

    // --------------------------
    // 通用样式定义（复用原有风格）
    // --------------------------
    const int BTN_WIDTH = 200;    // 按钮宽度（保持原有）
    const int BTN_HEIGHT = 50;    // 按钮高度（保持原有）
    const int TEXT_SIZE = 24;     // 文字大小（保持原有）
    const sf::Color BTN_NORMAL = sf::Color(200, 200, 200); // 正常按钮颜色
    const sf::Color BTN_HOVER = sf::Color(100, 100, 100);  // 选中按钮颜色
    const sf::Color BTN_BORDER = sf::Color(50, 50, 50);    // 按钮边框颜色
    const sf::Color TEXT_COLOR = sf::Color::Black;         // 文字颜色

    // --------------------------
    // 根据当前状态绘制对应菜单
    // --------------------------
    if (currentState == MenuState::MAIN) 
    {
        // 1. 绘制主菜单标题
        sf::Text title("labyrinth", font, 48);
        title.setFillColor(sf::Color::Black);
        sf::FloatRect titleRect = title.getLocalBounds();
        // 标题居中显示（顶部偏移100像素）
        title.setPosition(
            (WINDOW_WIDTH - titleRect.width) / 2,
            100  // 固定顶部位置，避免依赖btnBounds
        );
        window.draw(title);

        // 2. 计算主菜单按钮位置（居中排列，垂直间距60像素）
        std::vector<sf::FloatRect> btnBounds;
        int startY = 250; // 第一个按钮的Y坐标
        for (size_t i = 0; i < mainOptions.size(); ++i) 
        {
            float x = (WINDOW_WIDTH - BTN_WIDTH) / 2; // 水平居中
            float y = startY + i * (BTN_HEIGHT + 10); // 垂直排列，间距10
            btnBounds.push_back(sf::FloatRect(x, y, BTN_WIDTH, BTN_HEIGHT));
        }

        // 3. 绘制主菜单按钮和文字
        for (size_t i = 0; i < mainOptions.size(); ++i) 
        {
            // 按钮背景
            sf::RectangleShape btn(sf::Vector2f(BTN_WIDTH, BTN_HEIGHT));
            btn.setPosition(btnBounds[i].left, btnBounds[i].top);
            btn.setFillColor(i == selectedIndex ? BTN_HOVER : BTN_NORMAL);
            btn.setOutlineColor(BTN_BORDER);
            btn.setOutlineThickness(3);

            // 选中时放大效果
            if (i == selectedIndex) 
            {
                btn.setScale(1.05f, 1.05f);
                // 放大后重新居中（抵消缩放导致的偏移）
                btn.setPosition(
                    btnBounds[i].left - (BTN_WIDTH * 0.05f) / 2,
                    btnBounds[i].top - (BTN_HEIGHT * 0.05f) / 2
                );
            }
            window.draw(btn);

            // 按钮文字（居中显示在按钮内）
            sf::Text text(mainOptions[i], font, TEXT_SIZE);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setPosition(
                btnBounds[i].left + (BTN_WIDTH - textRect.width) / 2,
                btnBounds[i].top + (BTN_HEIGHT - textRect.height) / 2 - 5
            );
            text.setFillColor(TEXT_COLOR);
            window.draw(text);
        }

    } 
    else if (currentState == MenuState::MAP_SELECT) 
    {
        // 1. 绘制地图选择标题
        sf::Text title("Select Map", font, 48);
        title.setFillColor(sf::Color::Black);
        sf::FloatRect titleRect = title.getLocalBounds();
        title.setPosition(
            (WINDOW_WIDTH - titleRect.width) / 2,
            100  // 与主菜单标题同高度，保持一致性
        );
        window.draw(title);

        // 2. 计算地图选项按钮位置（居中排列，样式与主菜单一致）
        std::vector<sf::FloatRect> mapBtnBounds;
        int startY = 250; // 与主菜单第一个按钮同高度
        for (size_t i = 0; i < mapOptions.size(); ++i) 
        {
            float x = (WINDOW_WIDTH - BTN_WIDTH) / 2; // 水平居中（与主菜单按钮同宽）
            float y = startY + i * (BTN_HEIGHT + 10); // 垂直排列
            mapBtnBounds.push_back(sf::FloatRect(x, y, BTN_WIDTH, BTN_HEIGHT));
        }

        // 3. 绘制地图选项按钮和文字（复用主菜单的按钮样式）
        for (size_t i = 0; i < mapOptions.size(); ++i) 
        {
            // 按钮背景
            sf::RectangleShape btn(sf::Vector2f(BTN_WIDTH, BTN_HEIGHT));
            btn.setPosition(mapBtnBounds[i].left, mapBtnBounds[i].top);
            btn.setFillColor(i == selectedIndex ? BTN_HOVER : BTN_NORMAL);
            btn.setOutlineColor(BTN_BORDER);
            btn.setOutlineThickness(3);

            // 选中时放大效果（与主菜单一致）
            if (i == selectedIndex) 
            {
                btn.setScale(1.05f, 1.05f);
                btn.setPosition(
                    mapBtnBounds[i].left - (BTN_WIDTH * 0.05f) / 2,
                    mapBtnBounds[i].top - (BTN_HEIGHT * 0.05f) / 2
                );
            }
            window.draw(btn);

            // 地图选项文字
            sf::Text text(mapOptions[i], font, TEXT_SIZE);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setPosition(
                mapBtnBounds[i].left + (BTN_WIDTH - textRect.width) / 2,
                mapBtnBounds[i].top + (BTN_HEIGHT - textRect.height) / 2 - 5
            );
            text.setFillColor(TEXT_COLOR);
            window.draw(text);
        }

        // 4. 绘制返回提示（提示用户按ESC返回主菜单）
        sf::Text hint("Press ESC to return to main menu", font, 18);
        sf::FloatRect hintRect = hint.getLocalBounds();
        hint.setPosition(
            (WINDOW_WIDTH - hintRect.width) / 2,
            WINDOW_HEIGHT - 50  // 底部位置
        );
        hint.setFillColor(sf::Color(100, 100, 100)); // 灰色提示文字
        window.draw(hint);
    }
}