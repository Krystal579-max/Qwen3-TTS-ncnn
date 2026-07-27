# Build & Deployment Guide

## Prerequisites

| Tool | Minimum Version |
|---|---|
| **CMake** | >= 3.15 |
| **ncnn** | Installed system-wide or locally |
| **Compiler (Linux)** | GCC 9+ |
| **Compiler (Windows)** | MSVC 2019+ |

---

## Compilation Steps

### 1. Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/Qwen3-TTS-ncnn.git
cd Qwen3-TTS-ncnn
```

### 2. Create a Build Directory

```bash
mkdir build && cd build
```

### 3. Configure and Build

#### Linux (GCC/Clang)

```bash
cmake ..
make -j4
```

#### Windows (MSVC)

```bash
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

---

## Docker Deployment

The easiest way to deploy Qwen3-TTS-ncnn is by using the pre-built Docker image.

### Build the Docker Image

```bash
docker build -t ghcr.io/YOUR_USERNAME/qwen3-tts-ncnn:latest -f Krystal579-max.dockerfile .
```

### Pull from GitHub Packages

```bash
docker pull ghcr.io/YOUR_USERNAME/qwen3-tts-ncnn:latest
```

### Run Inference

```bash
docker run --rm \
  -v $(pwd)/models:/models \
  -v $(pwd)/output:/output \
  ghcr.io/YOUR_USERNAME/qwen3-tts-ncnn:latest \
  --model /models/qwen3_fp32 \
  --text "Hello, world!" \
  --output /output/result.wav
```

---

## CI/CD Pipeline

The repository includes a GitHub Actions workflow (`.github/workflows/publish-docker.yml`) that automatically:

1. **Triggers** on pushes to the `main` branch
2. **Builds** the Docker image using the multi-stage Dockerfile
3. **Pushes** the image to GitHub Packages with both `latest` and commit-SHA tags

---

## Node.js Integration

A Node.js client wrapper is available in the `qwen3-tts-node-client/` directory. It provides:

- `generateSpeech(text, outputFileName, dockerImage)` — Promise-based API
- Express.js web server with `POST /synthesize` endpoint
- Automatic output directory management and error handling

```bash
cd qwen3-tts-node-client
npm install
node index.js "Hello, world!" output.wav
```