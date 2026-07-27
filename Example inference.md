## Model Conversion

See `python_scripts/export_model.py` to convert the original PyTorch model to ncnn format using `pnnx`.

## Verification

Run `python_scripts/verify.py` to ensure numerical consistency between PyTorch and ncnn outputs.

## License & Acknowledgments

This project references the architecture of [nihui/ncnn_llm](https://github.com/nihui/ncnn_llm).
Original model by [QwenLM/Qwen3-TTS](https://github.com/QwenLM/Qwen3-TTS).
Inference engine by [Tencent/ncnn](https://github.com/Tencent/ncnn).
