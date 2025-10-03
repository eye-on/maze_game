#ifndef MENU_HPP
#define MENU_HPP

#include "../utils/Common.hpp"
#include <string>
#include <vector>

class Graphics; 

class Menu 
{
private:
    std::vector<std::string> options;  // 菜单选项
    int selectedIndex;                 // 当前选中的选项索引

public:
    Menu();

    // 处理菜单输入
    int handleInput();

    // 绘制菜单（通过Graphics）
    void draw(Graphics& graphics);

    // Getter
    int getSelectedMap() const { return selectedIndex - 2; } // 地图选项从索引2开始
};

#endif // MENU_HPP