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
    menu.setActive(true);
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
            graphics.getWindow().close();
        } 
        else if (event.type == sf::Event::KeyPressed) 
        {
            // 胜利/失败状态下按Enter切换到菜单
            if ((state == GameState::GAME_OVER || state == GameState::WIN) &&  event.key.code == sf::Keyboard::Enter) 
            {
                menu.setActive(true);
                state = GameState::MENU;
                
                sf::Event tempEvent;
                while (graphics.getWindow().pollEvent(tempEvent)) 
                {
                    // 什么都不做，只为清空队列   
                }
            }
            // 游戏中按ESC返回菜单
            else if (state == GameState::PLAYING && event.key.code == sf::Keyboard::Escape) 
            {
                while (graphics.pollEvent(event));
                state = GameState::MENU;
                menu.setActive(true);
            }
            else if (state == GameState::MENU) 
            {
                menu.handleEvent(event);
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) 
        {
            if (state == GameState::MENU) 
            {
                menu.handleEvent(event);
            }
        }
    }

    if (state == GameState::MENU) 
    {
        // 传入窗口引用给handleInput
        int menuResult = menu.handleInput(graphics.getWindow());
        if (menuResult == 0) { 
            menu.setActive(false);
            resetGame();
            state = GameState::PLAYING;
        } else if (menuResult == 1) { 
            graphics.getWindow().close();
        }
    } 
    else if (state == GameState::PLAYING) 
    {
        // 玩家移动 - 按键按下时只移动一次
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !wPressed) 
        {
            player.move(Direction::UP, map);
            wPressed = true;
        } 
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            wPressed = false;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !sPressed) 
        {
            player.move(Direction::DOWN, map);
            sPressed = true;
        } 
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            sPressed = false;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !aPressed) 
        {
            player.move(Direction::LEFT, map);
            aPressed = true;
        } 
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            aPressed = false;
        }
    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !dPressed) 
        {
            player.move(Direction::RIGHT, map);
            dPressed = true;
        } 
        else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            dPressed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
        {
            state = GameState::MENU; // 退回菜单
        }
    }
}

void Game::update() 
{
    // 计算当前帧的时间差（秒）
    float deltaTime = gameClock.restart().asSeconds();

    // 更新陷阱状态
    trapManager.update(deltaTime, player, map);

    // 原有碰撞检测逻辑...
    trapManager.checkCollisions(player);
    
    // 原有探索区域更新...
    map.revealArea(player.getX(), player.getY(), 2);
    
    // 原有游戏结束检测...
    if (!player.isAlive()) 
    {
        state = GameState::GAME_OVER;
    }
    if (player.getX() == map.getEndX() && player.getY() == map.getEndY()) 
    {
        state = GameState::WIN;
    }
}

void Game::render() 
{
    graphics.clear(sf::Color(20, 20, 30));


    if (state == GameState::MENU) 
    {
        // 传入窗口引用给draw
        graphics.drawMenuBackground();
        menu.draw(graphics.getWindow());
    } 
    else if (state == GameState::PLAYING) 
    {
    graphics.drawMap(map);
    graphics.drawTraps(trapManager, map);  
    graphics.drawPlayer(player);
    graphics.drawGameStatus(player, false);
    }
    else if (state == GameState::GAME_OVER) 
    {
        graphics.drawText(
            "Game Over! Press Enter to return to menu", 
            CELL_SIZE,  // 左边距1个单元格
            CELL_SIZE / 2  // 顶部边框内（y坐标在迷宫上方）
        );
        menu.setActive(true);
    } 
    else if (state == GameState::WIN) 
    {
        graphics.drawText(
            "You Win! Press Enter to return to menu", 
            CELL_SIZE, 
            CELL_SIZE / 2  // 顶部边框内
        );
        menu.setActive(true);
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