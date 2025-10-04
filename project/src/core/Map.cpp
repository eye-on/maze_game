#include "../../include/core/Map.hpp"
#include <fstream>
#include <iostream>

Map::Map() : width(MAP_WIDTH), height(MAP_HEIGHT), startX(1), startY(1), endX(MAP_WIDTH-2), endY(MAP_HEIGHT-2) 
{
    // 初始化探索状态为未探索
    explored.resize(height, std::vector<bool>(width, false));
}


bool Map::loadFromFile(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to load map: " << filePath << std::endl;
        return false;
    }

    grid.clear();
    explored.clear();  // 清空探索状态
    explored.resize(height, std::vector<bool>(width, false));  // 二度初始化
    std::string line;
    int y = 0;

    while (std::getline(file, line) && y < height) 
    {
        std::vector<MapElement> row;
        for (int x = 0; x < line.size() && x < width; x++) 
        {
            char c = line[x];
            if (c == '1') 
            {
                row.push_back(MapElement::WALL);
            } 
            else if (c == '0') 
            {
                row.push_back(MapElement::PATH);
            } 
            else if (c == 'S') 
            {  // 起点
                row.push_back(MapElement::PATH);
                startX = x;
                startY = y;
            } 
            else if (c == 'E') 
            {  // 终点
                row.push_back(MapElement::PATH);
                endX = x;
                endY = y;
            }
        }
        grid.push_back(row);
        y++;
    }

    file.close();
    return true;
}
void Map::revealArea(int centerX, int centerY, int radius) 
{
    for (int y = std::max(0, centerY - radius); y <= std::min(height - 1, centerY + radius); y++) 
    {
        for (int x = std::max(0, centerX - radius); x <= std::min(width - 1, centerX + radius); x++) 
        {
            explored[y][x] = true;
        }
    }
}
MapElement Map::getElement(int x, int y) const 
{
    if (x < 0 || x >= width || y < 0 || y >= height) 
    {
        return MapElement::WALL; // 边界外视为墙
    }
    return grid[y][x];
}

bool Map::isPositionValid(int x, int y) const 
{
    // 位置在地图内且是通路
    return (x >= 0 && x < width && y >= 0 && y < height) 
        && (grid[y][x] == MapElement::PATH);
}
bool Map::isExplored(int x, int y) const 
{
    if (x < 0 || x >= width || y < 0 || y >= height) 
    {
        return false;
    }
    return explored[y][x];
}