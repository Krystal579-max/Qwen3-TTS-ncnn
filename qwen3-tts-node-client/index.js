// index.js
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

/**
 * 使用 Qwen3-TTS-ncnn Docker 镜像生成语音
 * @param {string} text - 要合成的文本
 * @param {string} outputFileName - 输出 WAV 文件名
 * @param {string} dockerImage - Docker 镜像名称 (例如: ghcr.io/your-username/qwen3-tts-ncnn:latest)
 * @returns {Promise<string>} - 返回生成的 WAV 文件路径
 */
function generateSpeech(text, outputFileName, dockerImage) {
  return new Promise((resolve, reject) => {
    // 确保输出目录存在
    const outputDir = path.join(__dirname, 'output');
    if (!fs.existsSync(outputDir)) {
      fs.mkdirSync(outputDir);
    }

    const outputPath = path.join(outputDir, outputFileName);

    // 构建 Docker 命令
    const dockerCommand = `docker run --rm \
      -v ${outputDir}:/output \
      ${dockerImage} \
      --text "${text}" \
      --output /output/${outputFileName}`;

    console.log(`正在执行命令: ${dockerCommand}`);

    // 执行 Docker 命令
    exec(dockerCommand, (error, stdout, stderr) => {
      if (error) {
        console.error(`执行出错: ${error}`);
        reject(error);
        return;
      }
      if (stderr) {
        console.error(`标准错误输出: ${stderr}`);
        reject(new Error(stderr));
        return;
      }
      console.log(`标准输出: ${stdout}`);
      console.log(`语音合成成功! 文件已保存至: ${outputPath}`);
      resolve(outputPath);
    });
  });
}

// --- 使用示例 ---
const textToSynthesize = "你好，世界！这是使用 Qwen3-TTS-ncnn 通过 Node.js 生成的语音。";
const outputFileName = "hello_world.wav";
const dockerImage = "ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest"; // 请替换为你的镜像地址

generateSpeech(textToSynthesize, outputFileName, dockerImage)
  .then((filePath) => {
    console.log(`文件路径: ${filePath}`);
  })
  .catch((err) => {
    console.error("生成语音失败:", err);
  });