# Qwen3-TTS-ncnn Wiki

Welcome to the development wiki for **Qwen3-TTS-ncnn**. This project aims to provide a high-performance, cross-platform C++ implementation of the Qwen3-TTS model using the ncnn framework.

## 🚀 Quick Links

- [Model Architecture & Conversion](WIKI_Model_Architecture.md)
- [C++ Implementation Details](WIKI_C++_Implementation.md)
- [Build & Deployment Guide](WIKI_Build_Deployment.md)
- [Troubleshooting](WIKI_Troubleshooting.md)

---

## Overview

Qwen3-TTS-ncnn ports the Qwen3-TTS model to the `ncnn` framework, enabling efficient deployment on edge devices (Windows, Linux, Android, etc.) without heavy Python dependencies. It leverages `pnnx` for model conversion and implements the LLM decoding pipeline in pure C++.

### Key Features

- **Cross-Platform**: Native C++ support for Linux and Windows.
- **Zero Dependencies**: Relies only on `ncnn` and the C++ Standard Library.
- **High Precision**: Aligns output with PyTorch reference (FP32).
- **Efficient**: Optimized KV Cache management for streaming synthesis.

### Repository Structure

```
Qwen3-TTS-ncnn/
├── src/
│   ├── main.cpp          # Entry point, argument parsing, file I/O
│   ├── qwen3_tts.cpp     # Core inference engine class
│   └── pipeline.cpp      # Data preprocessing and post-processing
├── CMakeLists.txt        # Build configuration
├── Dockerfile            # Docker build configuration
├── .github/
│   └── workflows/        # CI/CD automation
└── models/               # Converted model files (.param, .bin)
```

### License

This project is licensed under the Apache-2.0 License.