<div align="center">

# Qwen3-TTS-ncnn 🎙️

[![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)](https://github.com/QwenLM/Qwen3-TTS)
[![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus)](https://isocpp.org/)
[![ncnn](https://img.shields.io/badge/ncnn-framework-BC2E2F)](https://github.com/Tencent/ncnn)
[![Docker](https://img.shields.io/badge/docker-ready-2496ED?logo=docker)](Krystal579-max.dockerfile)

**High-performance inference of Qwen3-TTS on CPU/GPU using ncnn.**

🚀 纯 C++ 实现 · 🐳 Docker 一键部署 · ⚡ KV Cache 优化推理

</div>

---

## 📖 项目简介

**Qwen3-TTS-ncnn** 将通义千问 [Qwen3-TTS](https://github.com/QwenLM/Qwen3-TTS) 模型移植到腾讯 [ncnn](https://github.com/Tencent/ncnn) 框架，提供高性能、跨平台的纯 C++ 推理实现。无需 Python 依赖，适合边缘设备（Linux、Windows、Android）部署。

通过 `pnnx` 完成模型转换，实现完整的 LLM 解码流水线，支持流式语音合成。

---

## ✨ 功能特性

| 特性 | 说明 |
|------|------|
| **跨平台** | 原生 C++ 支持 Linux 和 Windows，轻松移植到 Android |
| **零依赖** | 仅依赖 `ncnn` 和 C++ 标准库，无需 OpenCV / FFmpeg / PyBind11 |
| **高精度** | FP32 推理，输出与 PyTorch 参考实现严格对齐（MSE < 1e-5）|
| **高效推理** | 优化 KV Cache 管理，复杂度从 O(N²) 降至 O(N) |
| **Docker 部署** | 多阶段构建，最小化运行时镜像，CI/CD 自动发布 |
| **多语言接口** | 支持 Go / Node.js 客户端调用 |

---

## 🔧 快速开始

### 方式一：Docker 部署（推荐）

```bash
# 拉取预构建镜像
docker pull ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest

# 准备模型文件
mkdir -p models output
# 将 qwen3_fp32.param 和 qwen3_fp32.bin 放入 models/

# 运行推理
docker run --rm \
  -v $(pwd)/models:/models:ro \
  -v $(pwd)/output:/data \
  ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest \
  --model /models/qwen3_fp32 \
  --text "Hello, this is Qwen3-TTS running on ncnn."
```

生成的文件位于 `./output/output.wav`。

### 方式二：本地编译

#### 依赖要求

| 工具 | 最低版本 |
|------|---------|
| CMake | >= 3.15 |
| GCC / Clang (Linux) | GCC 9+ |
| MSVC (Windows) | 2019+ |
| ncnn | 最新版 |

#### 编译步骤

```bash
# 克隆仓库
git clone https://github.com/Krystal579-max/Qwen3-TTS-ncnn.git
cd Qwen3-TTS-ncnn

# 创建构建目录
mkdir build && cd build

# Linux
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Windows (MSVC)
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

---

## 🐳 Docker 镜像

项目使用多阶段构建的 Docker 镜像，自动通过 GitHub Actions 发布到 GitHub Packages。

### 构建镜像

```bash
docker build -t ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest -f Krystal579-max.dockerfile .
```

### 从 GitHub Packages 拉取

```bash
docker pull ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest
```

---

## 📁 仓库结构

```
Qwen3-TTS-ncnn/
├── src/                          # C++ 源码
│   ├── main.cpp                  # 入口、参数解析、WAV 输出
│   ├── qwen3_tts.cpp             # 核心推理引擎
│   ├── qwen3_tts.h               # 引擎头文件
│   └── pipeline.cpp              # 数据预处理与后处理
├── python_scripts/               # 模型转换脚本
│   └── export_model.py           # PyTorch → ncnn 导出
├── .github/workflows/            # CI/CD 工作流
│   ├── codeql.yml                # 代码安全扫描
│   └── publish-docker.yml        # Docker 自动构建与发布
├── docs/                         # 文档
│   ├── WIKI_Home.md              # Wiki 首页
│   ├── WIKI_Model_Architecture.md
│   ├── WIKI_C++_Implementation.md
│   ├── WIKI_Build_Deployment.md
│   └── WIKI_Troubleshooting.md
├── qwen3-tts-node-client/        # Node.js 客户端
│   ├── index.js                  # 核心 API
│   ├── server.js                 # Express Web 服务
│   └── package.json
├── Krystal579-max.dockerfile     # Docker 构建文件
├── main.go                       # Go 客户端
├── CMakeLists.txt                # CMake 构建配置
├── LICENSE                       # Apache-2.0 许可证
└── Readme.md                     # 本文档
```

---

## 🌐 多语言客户端

### Go

```bash
go run main.go "你好，世界！" output/hello.wav
```

### Node.js

```bash
cd qwen3-tts-node-client
npm install
node index.js
```

或者启动 Web 服务：

```bash
node server.js
# POST http://localhost:3000/synthesize
# Body: {"text": "你好", "outputFileName": "hello.wav"}
```

---

## 🧠 模型架构

Qwen3-TTS 模型由两个核心组件构成：

1. **语言模型 (LM)** — 基于 Transformer 架构，自回归预测离散音频 token
2. **声码器 (Vocoder)** — 将离散 token 解码为原始音频波形

转换流程：`PyTorch → torch.jit.trace → pnnx → ncnn (.param + .bin)`

详见 [模型架构文档](docs/WIKI_Model_Architecture.md)。

---

## 🤝 贡献指南

欢迎贡献代码、报告问题或提出新功能建议！

- 🐛 报告 Bug：使用 [Bug Report 模板](.github/ISSUE_TEMPLATE/bug_report.md)
- 💡 功能建议：使用 [Feature Request 模板](.github/ISSUE_TEMPLATE/feature_request.md)
- 📖 完善文档：PR 至 `docs/` 目录

---

## 📄 许可证

本项目基于 [Apache License 2.0](LICENSE) 开源。

## 🙏 致谢

- [QwenLM/Qwen3-TTS](https://github.com/QwenLM/Qwen3-TTS) — 原始模型
- [Tencent/ncnn](https://github.com/Tencent/ncnn) — 推理引擎
- [nihui/ncnn_llm](https://github.com/nihui/ncnn_llm) — 架构参考