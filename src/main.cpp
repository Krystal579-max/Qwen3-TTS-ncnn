#include <iostream>
#include <fstream>
#include "qwen3_tts.h"

// Simple WAV header writer
void write_wav(const std::string& filename, const std::vector<float>& data, int sample_rate = 22050) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return;

    int num_samples = data.size();
    int byte_rate = sample_rate * 2; // 16-bit
    
    // RIFF header
    file.write("RIFF", 4);
    int chunk_size = 36 + num_samples * 2;
    file.write((const char*)&chunk_size, 4);
    file.write("WAVE", 4);

    // fmt subchunk
    file.write("fmt ", 4);
    int subchunk1_size = 16;
    file.write((const char*)&subchunk1_size, 4);
    short audio_format = 1; // PCM
    file.write((const char*)&audio_format, 2);
    short num_channels = 1;
    file.write((const char*)&num_channels, 2);
    file.write((const char*)&sample_rate, 4);
    file.write((const char*)&byte_rate, 4);
    short block_align = 2;
    file.write((const char*)&block_align, 2);
    short bits_per_sample = 16;
    file.write((const char*)&bits_per_sample, 2);

    // data subchunk
    file.write("data", 4);
    int subchunk2_size = num_samples * 2;
    file.write((const char*)&subchunk2_size, 4);

    // Write PCM data (clamp float -1.0 to 1.0 to int16)
    for (float sample : data) {
        short s = (short)(std::max(-1.0f, std::min(1.0f, sample)) * 32767);
        file.write((const char*)&s, 2);
    }

    file.close();
    std::cout << "Audio saved to " << filename << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " --model <model_path> --text <text>" << std::endl;
        return 1;
    }

    std::string model_path = argv[2]; // Simplified parsing
    std::string text = argv[4];

    Qwen3TTSEngine engine;
    if (engine.load(model_path + ".param", model_path + ".bin") != 0) {
        return -1;
    }

    std::cout << "Generating audio for text: " << text << std::endl;
    std::vector<float> audio = engine.generate(text);

    if (!audio.empty()) {
        write_wav("output.wav", audio);
    } else {
        std::cerr << "Generation failed." << std::endl;
        return -1;
    }

    return 0;
}
