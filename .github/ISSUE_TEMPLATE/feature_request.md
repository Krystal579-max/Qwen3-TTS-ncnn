---
name: Feature Request
about: Suggest an idea for Qwen3-TTS-ncnn
title: "[Feature] Add Vulkan backend support for GPU acceleration"
labels: enhancement
assignees: ''
---

## Is your feature request related to a problem? Please describe.

A clear and concise description of what the problem is. For example:

The current CPU-only implementation works well, but achieving real-time factor (RTF < 0.1) is difficult on lower-end ARM devices (e.g., Raspberry Pi).

## Describe the solution you'd like

A clear and concise description of what you want to happen. For example:

I would like to enable the ncnn Vulkan backend to utilize the GPU for inference. This likely involves:

1. Adding a flag in the C++ code: `opt.use_vulkan_compute = true;`
2. Updating CMake to find the Vulkan SDK and link ncnn with Vulkan support.
3. Documentation on how to build ncnn with Vulkan enabled.

## Describe alternatives you've considered

A clear and concise description of any alternative solutions or features you've considered.

- Using ARM CPU optimizations (currently enabled by default in ncnn).
- Quantization to INT8 (planned for later).

## Additional Context

Add any other context or screenshots about the feature request here.

I can help contribute to the CMake updates if the maintainers are open to this feature.