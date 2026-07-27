docker run --rm \
  -v $(pwd)/models:/models \
  -v $(pwd)/output:/output \
  ghcr.io/Krystal579-max/Qwen3-TTS-ncnn/qwen3-tts-ncnn:latest \
  --model /models/qwen3_fp32 \
  --text "Hello, this is Qwen3-TTS running on ncnn." \
  --output /output/result.wav