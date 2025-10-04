#include "../../include/core/Game.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

Game::Game() 
    : state(GameState::MENU), 
      player(0, 0),  // 初始位置会在resetGame中更新
      currentMapIndex(0) 
{
    // 初始化图形和菜单
    if (!graphics.isWindowOpen()) 
    {
        std::cerr << "Failed to initialize graphics" << std::endl;
        exit(1);
    }
}

void Game::run() 
{
    while (graphics.isWindowOpen()) 
    {
        handleInput();
        if (state == GameState::PLAYING) 
        {
            update();
        }
        render();
    }
}

void Game::handleInput() 
{
    sf::Event event;
    while (graphics.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed) 
        {
        // 错误：graphics.~Graphics(); 
        graphics.getWindow().close();
        }
    }

    if (state == GameState::MENU) 
    {
        int menuResult = menu.handleInput();
        if (menuResult == 0) 
        { // 开始游戏
            resetGame();
            state = GameState::PLAYING;
        } 
        else if (menuResult == 1) 
        { 
        graphics.getWindow().close(); 
        }
        else if (menuResult >= 2) 
        { // 选择地图（2=map1,3=map2）
            currentMapIndex = menuResult - 2;
        }
    } 
    else if (state == GameState::PLAYING) 
    {
        // 玩家移动
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
        {
            player.move(Direction::UP, map);
        } 
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
        {
            player.move(Direction::DOWN, map);
        } 
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
        {
            player.move(Direction::LEFT, map);
        } 
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
        {
            player.move(Direction::RIGHT, map);
        } 
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
        {
            state = GameState::MENU; // 退回菜单
        }
    } 
    else if (state == GameState::GAME_OVER || state == GameState::WIN) 
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) 
        {
            state = GameState::MENU; // 回到菜单
        }
    }
}

void Game::update() 
{
    // 检测陷阱碰撞
    trapManager.checkCollisions(player);

    // 检测游戏结束（生命值为0）
    if (!player.isAlive()) 
    {
        state = GameState::GAME_OVER;
    }

    // 检测胜利（到达终点）
    if (player.getX() == map.getEndX() && player.getY() == map.getEndY()) 
    {
        state = GameState::WIN;
    }
}

void Game::render() 
{
    graphics.clear();

    if (state == GameState::MENU) 
    {
        graphics.drawMenuBackground();
        menu.draw(graphics);
    } 
    else if (state == GameState::PLAYING) 
    {
        graphics.drawMap(map);
        graphics.drawTraps(trapManager);
        graphics.drawPlayer(player);
        graphics.drawGameStatus(player, false);
    } 
    else if (state == GameState::GAME_OVER) 
    {
        graphics.drawMap(map);
        graphics.drawText("Game Over! Press Enter to return to menu", 50, WINDOW_HEIGHT/2);
    } 
    else if (state == GameState::WIN) 
    {
        graphics.drawMap(map);
        graphics.drawText("You Win! Press Enter to return to menu", 50, WINDOW_HEIGHT/2);
    }

    graphics.display();
}

void Game::resetGame() 
{
    // 根据currentMapIndex加载对应地图文件
    std::string mapPath = (currentMapIndex == 0) ? "assets/maps/map1.txt" : "assets/maps/map2.txt";
    if (!map.loadFromFile(mapPath)) 
    {
        std::cerr << "Failed to load map: " << mapPath << std::endl;
        state = GameState::MENU; // 加载失败返回菜单
        return;
    }
    // 初始化玩家（使用地图的起点）
    player = Player(map.getStartX(), map.getStartY());
    // 生成陷阱
    trapManager.generateTraps(5, player, map); // 假设生成5个陷阱
}