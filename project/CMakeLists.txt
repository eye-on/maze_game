# 第一步：指定最低CMake版本（必须在最前面）
cmake_minimum_required(VERSION 3.10)

# 第二步：定义项目名称（紧跟在cmake_minimum_required之后）
project(MazeGame)

# 第三步：设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 第四步：设置可执行文件输出目录（build/）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# 第五步：手动指定所有源文件路径（必须和你的实际文件路径完全一致！）
# 请根据你之前执行 `find src/ -name "*.cpp"` 的结果修改以下路径
set(SOURCES
    src/main.cpp
    src/core/Player.cpp
    src/core/Map.cpp
    src/core/Trap.cpp
    src/core/TrapManager.cpp
    src/core/Game.cpp
    src/ui/Menu.cpp
    src/ui/Graphics.cpp
    src/utils/Common.cpp
)

# 第六步：指定头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/ui
    ${CMAKE_SOURCE_DIR}/include/utils
)

# 第七步：查找SFML库
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# 第八步：生成可执行文件
add_executable(maze_game ${SOURCES})

# 第九步：链接SFML库
target_link_libraries(maze_game sfml-graphics sfml-window sfml-system)

add_custom_command(TARGET maze_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
)