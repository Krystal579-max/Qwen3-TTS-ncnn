// server.js (Express 示例)
const express = require('express');
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(express.json());

/**
 * 使用 Qwen3-TTS-ncnn Docker 镜像生成语音
 * @param {string} text - 要合成的文本
 * @param {string} outputFileName - 输出 WAV 文件名
 * @param {string} dockerImage - Docker 镜像名称
 * @returns {Promise<string>} - 返回生成的 WAV 文件路径
 */
function generateSpeech(text, outputFileName, dockerImage) {
  return new Promise((resolve, reject) => {
    const outputDir = path.join(__dirname, 'output');
    if (!fs.existsSync(outputDir)) {
      fs.mkdirSync(outputDir);
    }

    const outputPath = path.join(outputDir, outputFileName);
    const dockerCommand = `docker run --rm \
      -v ${outputDir}:/output \
      ${dockerImage} \
      --text "${text}" \
      --output /output/${outputFileName}`;

    exec(dockerCommand, (error, stdout, stderr) => {
      if (error) {
        reject(error);
        return;
      }
      if (stderr) {
        reject(new Error(stderr));
        return;
      }
      resolve(outputPath);
    });
  });
}

app.post('/synthesize', async (req, res) => {
  const { text, outputFileName } = req.body;

  if (!text || !outputFileName) {
    return res.status(400).send('缺少 text 或 outputFileName 参数');
  }

  try {
    const filePath = await generateSpeech(
      text,
      outputFileName,
      "ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest"
    );
    res.download(filePath, (err) => {
      if (err) {
        res.status(500).send('文件下载失败');
      }
    });
  } catch (error) {
    res.status(500).send('语音合成失败: ' + error.message);
  }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`服务器运行在 http://localhost:${PORT}`);
});