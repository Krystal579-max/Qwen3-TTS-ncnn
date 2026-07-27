## 🐳 Docker Installation (Recommended)

The easiest way to run Qwen3-TTS-ncnn is by using our pre-built Docker image.

### Prerequisites
- Docker installed on your system.

### 1. Pull the Docker Image

```bash
docker pull ghcr.io/Krystal579-max/Qwen3-TTS-ncnn/qwen3-tts-ncnn:latest
```

### 2. Run the Inference

```bash
docker run --rm \
  -v $(pwd)/models:/models \
  -v $(pwd)/output:/output \
  ghcr.io/Krystal579-max/Qwen3-TTS-ncnn/qwen3-tts-ncnn:latest \
  --model /models/qwen3_fp32 \
  --text "This is a test." \
  --output /output/result.wav
```