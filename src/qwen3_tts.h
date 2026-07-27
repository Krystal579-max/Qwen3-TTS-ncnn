#ifndef QWEN3_TTS_H
#define QWEN3_TTS_H

#include <ncnn/mat.h>
#include <string>
#include <vector>

class Qwen3TTSEngine {
public:
    Qwen3TTSEngine();
    ~Qwen3TTSEngine();

    // Load model files (.param and .bin)
    int load(const std::string& param_path, const std::string& bin_path);

    // Generate audio from text
    // Returns raw PCM data (float32)
    std::vector<float> generate(const std::string& text);

private:
    ncnn::Net net;
    bool is_loaded;
    
    // Internal state for KV cache (simplified)
    int past_length;
    
    // Helper for text tokenization (Placeholder)
    std::vector<int> tokenize_text(const std::string& text);
    
    // Helper for audio decoding (Placeholder)
    std::vector<float> decode_tokens(const std::vector<int>& tokens);
};

#endif // QWEN3_TTS_H
