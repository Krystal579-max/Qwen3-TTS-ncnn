# ==============================================================================
# Stage 1: Build Stage
# ==============================================================================
# 使用一个包含完整编译工具链的镜像作为构建环境
FROM ubuntu:22.04 AS builder

# 设置环境变量，避免交互式安装
ENV DEBIAN_FRONTEND=noninteractive

# 安装编译和运行时依赖
RUN apt-get update && apt-get install -y \
    git \
    cmake \
    g++ \
    libprotobuf-dev \
    protobuf-compiler \
    && rm -rf /var/lib/apt/lists/*

# 下载并编译 ncnn 库
WORKDIR /deps
RUN git clone --recursive https://github.com/Tencent/ncnn.git

WORKDIR /deps/ncnn
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -DNCNN_BUILD_TOOLS=OFF -DNCNN_BUILD_EXAMPLES=OFF .. && \
    make -j$(nproc)

# 编译 Qwen3-TTS-ncnn 项目
WORKDIR /app
COPY . .
RUN mkdir build && cd build && \
    cmake -Dncnn_DIR=/deps/ncnn/build/lib/cmake/ncnn .. && \
    make -j$(nproc)

# ==============================================================================
# Stage 2: Runtime Stage
# ==============================================================================
# 使用一个最小化的镜像作为运行环境
FROM ubuntu:22.04

# 安装运行时依赖
RUN apt-get update && apt-get install -y \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

# 从构建阶段复制二进制文件和 ncnn 库
COPY --from=builder /deps/ncnn/build/lib/libncnn.so /usr/local/lib/
COPY --from=builder /app/build/qwen3_tts_ncnn /usr/local/bin/qwen3_tts_ncnn
COPY --from=builder /app/models /models

# 设置库路径，确保程序能找到 ncnn.so
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# 设置工作目录
WORKDIR /data

# 定义默认的启动命令
ENTRYPOINT ["qwen3_tts_ncnn"]