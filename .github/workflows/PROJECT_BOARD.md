# 📋 Qwen3-TTS-ncnn Roadmap — Project Board

Create a GitHub Project Board named **"Qwen3-TTS-ncnn Roadmap"** and organize tasks into the following columns:

---

## Column 1: To Do

- [ ] Repository initialization (License, README, CMake setup)
- [ ] Analyze Qwen3-TTS PyTorch model architecture and dependencies
- [ ] Install pnnx and verify environment
- [ ] Implement basic WAV file writer in C++

---

## Column 2: In Progress

### Active Task: Model Conversion & Operator Alignment
- [ ] Trace PyTorch model
- [ ] Convert with pnnx
- [ ] Fix missing ops (e.g., custom Attention)

### Active Task: Tokenizer Porting
- [ ] Implement text-to-ID mapping
- [ ] Implement ID-to-audio decoding logic

---

## Column 3: Review / Testing

- [ ] Precision Verification — Compare PyTorch vs ncnn output
- [ ] Windows Build Test (MSVC)
- [ ] Linux Build Test (GCC/Clang)

---

## Column 4: Done

- [x] Fork/Clone reference projects (ncnn_llm)
- [x] Write technical documentation draft

---

## How to Use

1. Go to the repository's **Projects** tab
2. Click **"Create Project"** → select **"Board"** template
3. Name it: `Qwen3-TTS-ncnn Roadmap`
4. Create the four columns as shown above
5. Add cards by converting Issues or creating new notes
6. Drag cards across columns as work progresses