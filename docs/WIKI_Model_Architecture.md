# Model Architecture & Conversion

## The Qwen3-TTS Pipeline

The Qwen3-TTS model primarily consists of two components:

### 1. The Language Model (LM)
A Transformer-based architecture that processes text prompts and predicts discrete audio tokens autoregressively. This component handles the semantic understanding of the input text and generates a sequence of audio token representations.

### 2. The Vocoder / Decoder
Converts the discrete tokens back into raw audio waveforms. This component reconstructs the final audio signal from the token sequence produced by the language model.

---

## PNNX Conversion Strategy

To convert the PyTorch model to ncnn format, we follow a strict procedure to ensure operator compatibility:

### Step 1: Scripting

We use `torch.jit.trace` to capture the static graph of the model.

```python
model = Qwen3TTSModel()
model.eval()
dummy_input = torch.randn(1, 128)
traced_model = torch.jit.trace(model, dummy_input)
```

### Step 2: Conversion

We utilize `pnnx` to translate the traced model into ncnn parameters.

```bash
pnnx traced_model.pt inputshape=[1,128]
```

### Step 3: Operator Mapping

If the conversion fails due to unsupported operators (e.g., FlashAttention), we modify the Python source to use standard MultiheadAttention or decompose the op into supported primitives:

```
MatMul + Softmax + MatMul
```

### Verification

Run the verification script to ensure numerical consistency between PyTorch and ncnn outputs:

```bash
python scripts/verify.py
```

The output should match within a small MSE margin (< 1e-5).