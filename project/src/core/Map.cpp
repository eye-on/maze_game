#include "../../include/core/Map.hpp"
#include <fstream>
#include <iostream>

Map::Map() : width(MAP_WIDTH), height(MAP_HEIGHT), startX(1), startY(1), endX(MAP_WIDTH-2), endY(MAP_HEIGHT-2) {}

bool Map::loadFromFile(const std::string& filePath) 
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to load map: " << filePath << std::endl;
        return false;
    }

    grid.clear();
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