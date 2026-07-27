# Qwen3-TTS-ncnn: Node.js 客户端使用指南

本指南将介绍如何使用 Node.js 和 JavaScript 来调用 Qwen3-TTS-ncnn 的 Docker 镜像，从而在应用程序中实现文本转语音功能。

## 前提条件

- **Docker**: 确保系统上已安装 Docker。
- **Node.js**: 确保系统上已安装 Node.js（建议使用 LTS 版本）。
- **Qwen3-TTS-ncnn Docker 镜像**: 确保已构建并推送了 Docker 镜像到 GitHub Packages。

## 1. 项目初始化

```bash
# 创建项目目录
mkdir qwen3-tts-node-client
cd qwen3-tts-node-client

# 初始化 Node.js 项目
npm init -y

# 安装依赖（server.js 需要 express）
npm install express
```

## 2. 核心脚本：index.js

`index.js` 是调用 Docker 镜像生成语音的主脚本。它通过 `child_process.exec` 执行 Docker 命令，将文本转为语音并保存为 WAV 文件。

```javascript
const { exec } = require('child_process');
const fs = require('fs');
const path = require('path');

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
```

## 3. 运行脚本

```bash
node index.js
```

执行后，项目目录下会生成 `output/` 文件夹，其中包含合成的 WAV 文件。

## 4. 集成到 Web 应用（Express 示例）

`server.js` 展示如何将语音合成功能封装为 REST API 端点：

```javascript
const express = require('express');
const app = express();
app.use(express.json());

app.post('/synthesize', async (req, res) => {
  const { text, outputFileName } = req.body;
  if (!text || !outputFileName) {
    return res.status(400).send('缺少 text 或 outputFileName 参数');
  }

  try {
    const filePath = await generateSpeech(text, outputFileName, dockerImage);
    res.download(filePath);
  } catch (error) {
    res.status(500).send('语音合成失败: ' + error.message);
  }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
  console.log(`服务器运行在 http://localhost:${PORT}`);
});
```

启动 Web 服务器：

```bash
node server.js
```

然后发送 POST 请求进行语音合成：

```bash
curl -X POST http://localhost:3000/synthesize \
  -H "Content-Type: application/json" \
  -d '{"text": "你好，世界！", "outputFileName": "hello.wav"}'
```

## 5. 代码解释

### generateSpeech 函数

| 参数 | 说明 |
|---|---|
| `text` | 要合成的文本字符串 |
| `outputFileName` | 输出的 WAV 文件名（如 `result.wav`） |
| `dockerImage` | Docker 镜像完整标签（如 `ghcr.io/.../qwen3-tts-ncnn:latest`） |

### Docker 命令参数

| 参数 | 说明 |
|---|---|
| `--rm` | 容器退出后自动删除 |
| `-v ${outputDir}:/output` | 将本地输出目录挂载到容器内 |
| `--text` | 传递要合成的文本 |
| `--output` | 指定容器内输出文件路径 |

## 6. 目录结构

```
qwen3-tts-node-client/
├── index.js          # 核心脚本：调用 Docker 生成语音
├── server.js         # Express Web 服务器示例
├── package.json      # 项目配置
├── output/           # 生成的 WAV 文件存放目录
└── README.md         # 本文档
```

## 总结

通过 Node.js 的 `child_process` 模块，可以轻松地与 Docker 镜像进行交互，在 JavaScript 应用中集成 Qwen3-TTS-ncnn 的语音合成功能。这种方法简单、无需安装 C++ 编译工具链，并能充分利用 Docker 的隔离性和可移植性。