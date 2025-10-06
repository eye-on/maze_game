#ifndef MENU_HPP
#define MENU_HPP

#include "../utils/Common.hpp"
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
class Graphics; 
enum class MenuState { MAIN, MAP_SELECT };
class Menu 
{
private:
    bool upPressed = false;       // 上方向键按下事件标志
    bool downPressed = false;     // 下方向键按下事件标志
    bool returnPressed = false;   // 回车键按下事件标志
    bool leftMousePressed = false;// 鼠标左键按下事件标志
    MenuState currentState = MenuState::MAIN;
    std::vector<std::string> mainOptions = {"Start Game", "Image to Map", "Exit"};
    std::vector<std::string> mapOptions;
    int selectedIndex = 0;  // 0=Start, 1=Exit
    bool isActive = false;
    bool ignoreFirstInput = false;
    // 按钮样式配置（可自定义调整）
    const int BTN_WIDTH = 220;
    const int BTN_HEIGHT = 70;
    const int BTN_SPACING = 30;  // 按钮间距
    const sf::Color BTN_NORMAL = sf::Color(40, 40, 60);       // 常态：深色风暴
    const sf::Color BTN_HOVER = sf::Color(80, 80, 120);       // 悬停：霓虹炫彩
    const sf::Color BTN_BORDER = sf::Color(200, 200, 255);    // 边框：万里晴空
    const sf::Color TEXT_COLOR = sf::Color(255, 255, 255);    // 文字：纯净幻想
    const int TEXT_SIZE = 28;

    sf::Font font;  // 按钮文字字体
    // 预计算按钮位置（避免重复计算）
    std::vector<sf::FloatRect> btnBounds;

public:
    Menu();
    void loadMapList(const std::string& directory);
    int handleInput(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;  // 绘制按钮菜单
    void setActive(bool active);
    void handleEvent(const sf::Event& event);
    const std::vector<std::string>& getMapOptions() const 
    {
        return mapOptions;
    }
};

#endif // MENU_HPP