# Troubleshooting

## "Failed to load param file"

**Cause**: The `.param` file path is incorrect or corrupted.

**Fix**: Ensure the model files are in the correct directory and check the `CMakeLists.txt` file copy rules.

```bash
# Verify model files exist
ls -la ./models/
# Expected: qwen3_fp32.param, qwen3_fp32.bin
```

---

## "Segmentation Fault (core dumped)"

**Cause**: Usually a mismatch in the input tensor dimensions or an issue with the KV Cache memory allocation.

**Fix**: Check that `inputshape` in the `pnnx` command matches the C++ input dimensions in `qwen3_tts.cpp`.

```cpp
// Verify input dimensions in qwen3_tts.cpp
// The ncnn Extractor input shape must match pnnx conversion
ncnn::Mat input = ncnn::Mat::from_pointer(data, 1, 128);  // [1, 128]
```

---

## Windows Build Errors (LNK 2019)

**Cause**: The linker cannot find the ncnn library.

**Fix**: Ensure `ncnn_DIR` is set correctly in CMake or that the library path is explicitly linked in `target_link_libraries`.

```bash
# Configure with explicit ncnn path
cmake .. -Dncnn_DIR="C:/path/to/ncnn/build/lib/cmake/ncnn"
```

---

## Extractor Fails at Step N (Error Code -3)

**Cause**: The ncnn extractor encountered an error during inference. Error code -3 typically indicates a layer execution failure.

**Fix**:
- Verify the model files are compatible with your ncnn version
- Check for unsupported operations in the model graph
- Ensure the ncnn library version matches the build version

---

## Docker - Permission Denied

**Cause**: Docker daemon requires elevated privileges.

**Fix**:
```bash
# Add your user to the docker group
sudo usermod -aG docker $USER
# Log out and back in, or run:
newgrp docker
```

---

## Docker - Image Not Found

**Cause**: The Docker image has not been built or pushed to the registry.

**Fix**:
```bash
# Build the image locally
docker build -t ghcr.io/YOUR_USERNAME/qwen3-tts-ncnn:latest .

# Or pull from GitHub Packages
docker pull ghcr.io/YOUR_USERNAME/qwen3-tts-ncnn:latest
```

---

## Getting Help

If you encounter issues not covered here, please:

1. Check the [GitHub Issues](https://github.com/Krystal579-max/Qwen3-TTS-ncnn/issues) page
2. Open a new issue with:
   - Your operating system and compiler version
   - The exact command and error output
   - Steps to reproduce the problem
   - ncnn version and build configuration