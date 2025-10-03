#ifndef MAP_H
#define MAP_H

#include "../utils/common.hpp"
#include <vector>
#include <string>

class Map {
private:
    std::vector<std::vector<MapElement>> grid;  // 地图栅格数据
    int width, height;                          // 地图尺寸
    int startX, startY;                         // 玩家起始位置
    int endX, endY;                             // 迷宫终点位置

public:
    Map();

    // 从文件加载地图（格式：0=通路,1=墙,S=起点,E=终点）
    bool loadFromFile(const std::string& filePath);

    // 获取指定位置的元素
    MapElement getElement(int x, int y) const;

    // 检查位置是否合法（在地图内且是通路）
    bool isPositionValid(int x, int y) const;

    // Getter
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getStartX() const { return startX; }
    int getStartY() const { return startY; }
    int getEndX() const { return endX; }
    int getEndY() const { return endY; }
};

#endif // MAP_H