Qwen3-TTS-ncnn
<div align="center">
High-Performance, Cross-Platform Inference of Qwen3-TTS using ncnn
</div>

Overview

Qwen3-TTS-ncnn is a high-performance deployment solution for the Qwen3-TTS model. It leverages the powerful ncnn inference framework to run the model in pure C++, eliminating the dependency on heavy Python environments (PyTorch/Python).

This project achieves:

High Performance: 

Optimized KV Cache management for real-time streaming synthesis.

Zero Dependencies: 

Relies only on ncnn and the C++ Standard Library. No OpenCV, FFmpeg, or Python required.

Cross-Platform: 

Native support for Linux, Windows, macOS, and Android.

High Precision: 

Strict numerical alignment with the original PyTorch implementation (FP32).
Features

Autoregressive Decoding: 

Efficient LLM-style text-to-audio generation pipeline.

KV Cache Optimization: 

Implements standard KV Cache logic to reduce computational complexity from $O(N^2)$ to $O(N)$.

Pure C++ Implementation: 

Core Tokenizer and Inference Engine rewritten in C++ for maximum portability.

Streamlined Build: 

Easy CMake-based build system compatible with GCC, Clang, and MSVC.
Build Instructions

Prerequisites

CMake: 
Version 3.15 or higher.

C++ Compiler:

GCC 9+ / Clang 10+ (Linux)

Visual Studio 2019+ (Windows)

ncnn Library:

Install from source: Tencent/ncnn

use my system package manager (e.g., vcpkg, brew).

Note: Ensure you install ncnn with Vulkan support if you wish to use GPU acceleration.
Building from Source

Clone the repositorygit clone https://github.com/Krystal579-max/Qwen3-TTS-ncnn.git
cd Qwen3-TTS-ncnn

Create a build directorymkdir build && cd build

Configure with CMake# Linux / macOS

cmake .. -DCMAKE_BUILD_TYPE=Release

# Windows (Visual Studio)

cmake .. -G "Visual Studio 16 2019" -A x64

Compile# Linux / macOS

make -j4

# Windows

cmake --build . --config Release

The executable qwen3_tts_ncnn (or qwen3_tts_ncnn.exe) will be generated in the build directory.
Usage

Basic Inference

Run the compiled executable with the model path and input text.

# Linux / macOS

./qwen3_tts_ncnn \
  --model ../models/qwen3_tts_fp32 \
  --text "Hello, this is a test of Qwen3-TTS running on ncnn." \
  --output ../output.wav

# Windows

qwen3_tts_ncnn.exe \
  --model ..\models\qwen3_tts_fp32 \
  --text "Hello, this is a test of Qwen3-TTS running on ncnn." \
  --output ..\output.wav
  Command Line Arguments

Model Conversion (Developers)

To convert the original PyTorch model to ncnn format, you need to use the pnnx tool.

Install PNNX: 

Follow the guide here.

Run the Export Script:

cd python_scripts python export_model.py

Verification:

The script generates qwen3_tts_fp32.param and qwen3_tts_fp32.bin. Move these files to the models/ directory.

Note: Ensure your PyTorch model export handles all dynamic shapes correctly or sets a fixed sequence length for optimal ncnn performance.

Performance & Accuracy

Precision Alignment

We verified the numerical consistency between the PyTorch (CPU/Floating Point) and ncnn implementations.

Metric: 

Mean Squared Error (MSE) on output waveforms.

Result: 

MSE < $1 \times 10^{-5}$.

Conclusion: 

The ncnn implementation is perceptually identical to the original.

Benchmarks

Tested on Intel i5-10400F (6 Cores) / 16GB RAM / Ubuntu 22.04

Project Structure

Qwen3-TTS-ncnn/

├── CMakeLists.txt           # CMake build configuration

├── README.md                # This file

├── python_scripts/
│   ├── export_model.py      # PyTorch to ncnn conversion script

│   └── verify.py            # Precision verification tool

├── src/
│   ├── main.cpp             # CLI entry point

│   ├── qwen3_tts.cpp        # TTS Model class & inference engine

│   ├── qwen3_tts.h          # Header definitions

│   └── pipeline.cpp         # Minimal audio I/O and preprocessing

└── models/                  # Directory for .param and .bin files

Contributing

Contributions are welcome! This project is complex and involves low-level optimization.
Fork the repository.

Create a feature branch (git checkout -b feature/AmazingFeature).

Ensure my code compiles on at least Linux and Windows.

Verify that changes do not increase the MSE error rate significantly.

Commit my changes (git commit -m 'Add some AmazingFeature').

Push to the branch (git push origin feature/AmazingFeature).

Open a Pull Request.

Acknowledgments

Qwen Team for the excellent Qwen3-TTS model.

Tencent/ncnn for providing the high-performance inference framework.

nihui/ncnn_llm for the reference implementation of LLM decoding in ncnn.

futz12 for contributions to the ncnn ecosystem.

License

This project is licensed under the MIT License - see the LICENSE file for details.
