#ifndef IMAGE_CONVERTER_HPP
#define IMAGE_CONVERTER_HPP

#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

class ImageConverter 
{
public:
    // 将二值化图像转换为地图文件
    static bool convertImageToMap(const std::string& imagePath, const std::string& outputPath) 
    {
        sf::Image image;
        if (!image.loadFromFile(imagePath)) 
        {
            return false; // 图片加载失败
        }

        sf::Vector2u size = image.getSize();
        std::ofstream file(outputPath);
        if (!file.is_open()) 
        {
            return false; // 文件打开失败
        }

        int startX = -1, startY = -1; // 起点(S)坐标
        int endX = -1, endY = -1;     // 终点(E)坐标

        // 第一遍扫描：确定S和E的位置
        for (unsigned int y = 0; y < size.y; y++) 
        {
            for (unsigned int x = 0; x < size.x; x++) 
            {
                sf::Color color = image.getPixel(x, y);
                bool isWhite = (color.r == 255 && color.g == 255 && color.b == 255);
                
                // 左上角第一个白色像素作为起点(S)
                if (startX == -1 && isWhite) 
                {
                    startX = x;
                    startY = y;
                }
                // 右下角最后一个白色像素作为终点(E)
                if (isWhite) 
                {
                    endX = x;
                    endY = y;
                }
            }
        }

        // 若未找到白色像素，默认S和E的位置
        if (startX == -1) 
        {
            startX = startY = 0; // 默认为左上角
            endX = size.x - 1;
            endY = size.y - 1;   // 默认为右下角
        }

        // 第二遍扫描：生成地图内容
        for (unsigned int y = 0; y < size.y; y++) 
        {
            std::string line;
            for (unsigned int x = 0; x < size.x; x++) 
            {
                if (x == startX && y == startY) 
                {
                    line += 'S'; // 起点
                } 
                else if (x == endX && y == endY) 
                {
                    line += 'E'; // 终点
                } 
                else 
                {
                    // 黑色(0,0,0)→1（墙），白色(255,255,255)→0（路）
                    sf::Color color = image.getPixel(x, y);
                    line += (color.r == 0 && color.g == 0 && color.b == 0) ? '1' : '0';
                }
            }
            file << line << std::endl;
        }

        file.close();
        return true;
    }
};

#endif // IMAGE_CONVERTER_HPP