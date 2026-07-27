package main

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
)

const (
	// 默认 Docker 镜像名称（如需修改请替换为你的实际镜像标签）
	defaultImageName = "ghcr.io/Krystal579-max/qwen3-tts-ncnn:latest"
	// 本地模型目录（相对于当前工作目录）
	defaultModelDir = "./models"
	// 默认模型文件名前缀（不含 .param / .bin 扩展名）
	defaultModelName = "qwen3_fp32"
)

func main() {
	// ---------------------------------------------------------------------------
	// 命令行参数解析
	// ---------------------------------------------------------------------------
	if len(os.Args) < 3 {
		fmt.Fprintf(os.Stderr, "Usage: %s <\"text to synthesize\"> <output.wav> [model_name]\n", os.Args[0])
		fmt.Fprintf(os.Stderr, "\nExamples:\n")
		fmt.Fprintf(os.Stderr, "  %s \"Hello, world!\" hello.wav\n", os.Args[0])
		fmt.Fprintf(os.Stderr, "  %s \"你好，世界！\" chinese.wav qwen3_fp32\n", os.Args[0])
		os.Exit(1)
	}

	text := os.Args[1]
	outputFile := os.Args[2]

	modelName := defaultModelName
	if len(os.Args) >= 4 {
		modelName = os.Args[3]
	}

	// ---------------------------------------------------------------------------
	// 路径解析与校验
	// ---------------------------------------------------------------------------

	// 输出文件绝对路径
	absOutput, err := filepath.Abs(outputFile)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: invalid output path %q: %v\n", outputFile, err)
		os.Exit(1)
	}
	outputDir := filepath.Dir(absOutput)
	outputBase := filepath.Base(absOutput)

	// 确保输出目录存在
	if err := os.MkdirAll(outputDir, 0755); err != nil {
		fmt.Fprintf(os.Stderr, "Error: cannot create output directory %q: %v\n", outputDir, err)
		os.Exit(1)
	}

	// 模型目录绝对路径
	absModelDir, err := filepath.Abs(defaultModelDir)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: invalid model directory %q: %v\n", defaultModelDir, err)
		os.Exit(1)
	}

	// 检查模型目录是否存在
	modelParam := filepath.Join(absModelDir, modelName+".param")
	modelBin := filepath.Join(absModelDir, modelName+".bin")
	if _, err := os.Stat(modelParam); os.IsNotExist(err) {
		fmt.Fprintf(os.Stderr, "Error: model param file not found: %s\n", modelParam)
		os.Exit(1)
	}
	if _, err := os.Stat(modelBin); os.IsNotExist(err) {
		fmt.Fprintf(os.Stderr, "Error: model bin file not found: %s\n", modelBin)
		os.Exit(1)
	}

	// ---------------------------------------------------------------------------
	// Docker 命令构建
	// ---------------------------------------------------------------------------
	// 底层二进制 qwen3_tts_ncnn 接受 --model <prefix> --text <text>
	// 输出硬编码为容器内 /data/output.wav（WORKDIR 为 /data）
	// 我们将本地输出目录挂载到 /data，运行后从 output.wav 重命名为用户指定文件名
	imageName := defaultImageName

	// 允许通过环境变量 QWEN3_TTS_IMAGE 覆盖镜像名称
	if envImg := os.Getenv("QWEN3_TTS_IMAGE"); envImg != "" {
		imageName = envImg
	}

	args := []string{
		"run", "--rm",
		"-v", absModelDir + ":/models:ro",
		"-v", outputDir + ":/data",
		imageName,
		"--model", "/models/" + modelName,
		"--text", text,
	}

	cmd := exec.Command("docker", args...)

	// ---------------------------------------------------------------------------
	// 执行
	// ---------------------------------------------------------------------------
	fmt.Printf("Synthesizing text: %s\n", text)
	fmt.Printf("Using model:       %s\n", modelName)
	fmt.Printf("Output file:       %s\n", absOutput)
	fmt.Println("Running Docker container...")

	output, err := cmd.CombinedOutput()
	if err != nil {
		fmt.Fprintf(os.Stderr, "TTS synthesis failed:\n")
		fmt.Fprintf(os.Stderr, "  Error: %v\n", err)
		fmt.Fprintf(os.Stderr, "  Output:\n%s\n", output)
		os.Exit(1)
	}

	// ---------------------------------------------------------------------------
	// 处理输出文件（重命名 output.wav → 用户指定的文件名）
	// ---------------------------------------------------------------------------
	generatedWav := filepath.Join(outputDir, "output.wav")
	desiredPath := filepath.Join(outputDir, outputBase)

	if generatedWav != desiredPath {
		// 如果目标文件已存在，先删除
		if _, err := os.Stat(desiredPath); err == nil {
			if err := os.Remove(desiredPath); err != nil {
				fmt.Fprintf(os.Stderr, "Warning: could not remove existing file %q: %v\n", desiredPath, err)
			}
		}
		if err := os.Rename(generatedWav, desiredPath); err != nil {
			// 重命名失败时，将 output.wav 的路径作为结果
			fmt.Fprintf(os.Stderr, "Warning: could not rename output.wav to %q: %v\n", outputBase, err)
			fmt.Printf("Output file (default name): %s\n", generatedWav)
		} else {
			fmt.Printf("Output saved to: %s\n", desiredPath)
		}
	} else {
		fmt.Printf("Output saved to: %s\n", generatedWav)
	}

	// 打印 Docker 输出
	fmt.Printf("\nDocker output:\n%s\n", output)
}