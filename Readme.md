# Qwen3-TTS-ncnn

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey.svg)](https://github.com/QwenLM/Qwen3-TTS)

**High-performance inference of Qwen3-TTS on CPU/GPU using ncnn.**

This project ports the Qwen3-TTS model to the `ncnn` framework, enabling efficient deployment on edge devices (Windows, Linux, Android, etc.) without heavy Python dependencies. It leverages `pnnx` for model conversion and implements the LLM decoding pipeline in pure C++.

## Features

- **Cross-Platform:** Native C++ support for Linux and Windows.
- **Zero Dependencies:** Relies only on `ncnn` and the C++ Standard Library.
- **High Precision:** Aligns output with PyTorch reference (FP32).
- **Efficient:** Optimized KV Cache management for streaming synthesis.

## Build Instructions

### Prerequisites

- **CMake** (>= 3.15)
- **C++ Compiler** (GCC 9+, Clang, or MSVC 2019+)
- **ncnn Library:** Install from [Tencent/ncnn](https://github.com/Tencent/ncnn).
    
