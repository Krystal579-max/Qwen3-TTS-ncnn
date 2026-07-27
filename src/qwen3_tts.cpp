#include "qwen3_tts.h"
#include <iostream>
#include <algorithm>

Qwen3TTSEngine::Qwen3TTSEngine() : is_loaded(false), past_length(0) {
    // Configure ncnn options (threading, vulkan, etc.)
    ncnn::Option opt;
    opt.num_threads = 4;
    opt.use_vulkan_compute = false; // Set to true if ncnn is built with Vulkan
    ncnn::set_default_option(opt);
}

Qwen3TTSEngine::~Qwen3TTSEngine() {
    // Net is destroyed automatically
}

int Qwen3TTSEngine::load(const std::string& param_path, const std::string& bin_path) {
    if (net.load_param(param_path.c_str()) != 0) {
        std::cerr << "Failed to load param file: " << param_path << std::endl;
        return -1;
    }
    if (net.load_model(bin_path.c_str()) != 0) {
        std::cerr << "Failed to load bin file: " << bin_path << std::endl;
        return -1;
    }
    is_loaded = true;
    std::cout << "Model loaded successfully." << std::endl;
    return 0;
}

// MOCK Tokenizer: In a real scenario, port the Python tokenizer to C++
std::vector<int> Qwen3TTSEngine::tokenize_text(const std::string& text) {
    // This is a placeholder. 
    // Real implementation requires porting SentencePiece or BPE logic.
    std::vector<int> ids;
    // Example: simple hash (not valid for real TTS, just for compilation demo)
    for(char c : text) {
        ids.push_back((int)c);
    }
    return ids;
}

// MOCK Decoder: Vocoder logic
std::vector<float> Qwen3TTSEngine::decode_tokens(const std::vector<int>& tokens) {
    std::vector<float> audio;
    // Placeholder logic
    for(int t : tokens) {
        audio.push_back((float)(t % 100) / 100.0f);
    }
    return audio;
}

std::vector<float> Qwen3TTSEngine::generate(const std::string& text) {
    if (!is_loaded) {
        std::cerr << "Model not loaded!" << std::endl;
        return {};
    }

    std::vector<int> input_ids = tokenize_text(text);
    std::vector<int> generated_tokens;
    
    // Autoregressive Decoding Loop
    int max_len = input_ids.size() + 100; // Stop condition
    
    // Exctractor setup
    ncnn::Extractor ex = net.create_extractor();
    
    // Initial forward pass (Prompt processing)
    // For ncnn LLMs, we usually process the prompt and grow the KV Cache.
    // Simplified here to a loop for demonstration.
    
    for (int i = 0; i < max_len; ++i) {
        ncnn::Mat input_seq(1); // Batch size 1
        
        // For demo, we just feed the text length. 
        // In reality: feed the last generated token.
        int current_token = (i < input_ids.size()) ? input_ids[i] : generated_tokens.back();
        
        // Populate input mat
        // Note: ncnn::Mat is row-major by default usually, but check model export.
        // Here we assume input is a scalar or 1x1 vector for the next token.
        float* ptr = input_seq;
        ptr[0] = (float)current_token;

        // Set input
        // "input" is the name defined in your export script or default
        ex.input("in0", input_seq); 

        // Get output
        ncnn::Mat logits;
        int ret = ex.extract("out0", logits);
        
        if (ret != 0) {
            std::cerr << "Inference failed at step " << i << std::endl;
            break;
        }

        // Sampling (Greedy Search)
        // Find index of max value in logits
        int next_token = 0;
        float max_val = -10000.0f;
        const float* logit_ptr = logits;
        
        // Assuming vocab size is logits.w
        int vocab_size = logits.w;
        for (int j = 0; j < vocab_size; ++j) {
            if (logit_ptr[j] > max_val) {
                max_val = logit_ptr[j];
                next_token = j;
            }
        }
        
        generated_tokens.push_back(next_token);

        // Stop condition (e.g., End of Sentence token)
        if (next_token == 0) break; 
    }

    std::cout << "Generated " << generated_tokens.size() << " tokens." << std::endl;
    
    // Decode tokens to audio waveform
    return decode_tokens(generated_tokens);
}
