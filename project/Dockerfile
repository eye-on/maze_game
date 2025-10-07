# 使用Ubuntu 24.04作为基础镜像
FROM ubuntu:24.04

# 设置环境变量，避免交互提示
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表并安装依赖
RUN apt-get update && apt-get install -y \
    gcc-13 \
    g++-13 \
    cmake=3.28.* \
    make \
    && rm -rf /var/lib/apt/lists/*

# 设置默认编译器为GCC 13
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 \
    && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100

# 创建工作目录
WORKDIR /app

# 复制项目文件到容器
COPY . /app

# 创建构建目录并进行编译
RUN mkdir -p build && cd build \
    && cmake .. \
    && make

# 运行生成的可执行文件
CMD ["/app/build/maze_game"]