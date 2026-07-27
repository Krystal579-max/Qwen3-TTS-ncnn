# C++ Implementation Details

## Directory Structure

```
src/
├── main.cpp          # Entry point, argument parsing, and file I/O
├── qwen3_tts.cpp     # The core inference engine class
└── pipeline.cpp      # Data preprocessing and post-processing
```

### `main.cpp`
Handles command-line argument parsing, model loading, and WAV file output. It accepts `--model` and `--text` parameters, invokes the inference engine, and writes the generated audio to a file.

### `qwen3_tts.cpp`
Contains the core `Qwen3TTSEngine` class that:
- Loads the ncnn model (`.param` and `.bin` files)
- Manages the inference lifecycle
- Implements the KV Cache mechanism
- Generates audio from text input

### `pipeline.cpp`
Handles data preprocessing (tokenization, input encoding) and post-processing (audio decoding, waveform generation).

---

## KV Cache Implementation

To achieve efficient autoregressive generation, we implemented a Key-Value (KV) Cache mechanism similar to `ncnn_llm`.

### Phases

| Phase | Description |
|---|---|
| **Initialization** | The cache is empty. |
| **Prefill Phase** | Process the entire input text prompt in one forward pass to populate the KV Cache. |
| **Decode Phase** | For subsequent steps, feed only the last generated token. The `ncnn::Extractor` reuses the memory allocated for the KV Cache, reducing complexity from **O(N²) to O(N)**. |

### Benefits

- **Performance**: Significant reduction in per-token computation time
- **Memory Efficiency**: Reuses allocated memory across generation steps
- **Scalability**: Enables longer sequence generation without quadratic cost

---

## Dependency Management

We strictly minimize dependencies. The project only relies on:

| Dependency | Purpose |
|---|---|
| **ncnn** | Core inference engine |
| **C++ Standard Library** | File I/O, string manipulation, containers |
| **No Third-Party Libs** | We avoid OpenCV, FFmpeg, or PyBind11 |

This zero-dependency approach ensures easy compilation and deployment across platforms.