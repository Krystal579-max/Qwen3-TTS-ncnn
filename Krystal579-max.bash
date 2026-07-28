#!/usr/bin/env bash
# ==============================================================================
# Qwen3-TTS-ncnn Docker 构建与推送脚本
# 使用方法:
#   chmod +x Krystal579-max.bash
#   ./Krystal579-max.bash              # 仅构建镜像
#   ./Krystal579-max.bash --push       # 构建并推送镜像
#   ./Krystal579-max.bash --no-cache   # 不使用缓存构建
# ==============================================================================

set -euo pipefail

# ------------------------------------------------------------------------------
# 配置
# ------------------------------------------------------------------------------
REGISTRY="ghcr.io"
USERNAME="Krystal579-max"
IMAGE_NAME="qwen3-tts-ncnn"
TAG="latest"
FULL_IMAGE_TAG="${REGISTRY}/${USERNAME}/${IMAGE_NAME}:${TAG}"

# ------------------------------------------------------------------------------
# 颜色输出辅助函数
# ------------------------------------------------------------------------------
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

info()  { echo -e "${GREEN}[INFO]${NC}  $1"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $1"; }
error() { echo -e "${RED}[ERROR]${NC} $1"; }

# ------------------------------------------------------------------------------
# 参数解析
# ------------------------------------------------------------------------------
PUSH=false
BUILD_ARGS=()

for arg in "$@"; do
    case "$arg" in
        --push)
            PUSH=true
            shift
            ;;
        --no-cache)
            BUILD_ARGS+=("--no-cache")
            shift
            ;;
        *)
            error "未知参数: $arg"
            echo "用法: $0 [--push] [--no-cache]"
            exit 1
            ;;
    esac
done

# ------------------------------------------------------------------------------
# 前置检查
# ------------------------------------------------------------------------------
# 检查 Docker 是否已安装
if ! command -v docker &> /dev/null; then
    error "Docker 未安装，请先安装 Docker。"
    exit 1
fi

# 检查 Dockerfile 是否存在
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DOCKERFILE="${SCRIPT_DIR}/Krystal579-max.dockerfile"

if [ ! -f "$DOCKERFILE" ]; then
    error "未找到 Dockerfile，请确保 'Krystal579-max.dockerfile' 与脚本位于同一目录。"
    exit 1
fi

# ------------------------------------------------------------------------------
# 构建镜像
# ------------------------------------------------------------------------------
info "开始构建 Docker 镜像..."
info "Dockerfile: ${DOCKERFILE}"
info "镜像标签: ${FULL_IMAGE_TAG}"

docker build \
    -f "${DOCKERFILE}" \
    -t "${FULL_IMAGE_TAG}" \
    "${BUILD_ARGS[@]}" \
    "${SCRIPT_DIR}"

info "构建成功: ${FULL_IMAGE_TAG}"

# ------------------------------------------------------------------------------
# 推送镜像（可选）
# ------------------------------------------------------------------------------
if [ "$PUSH" = true ]; then
    info "正在推送镜像至 ${REGISTRY}..."

    # 检查是否已登录
    if ! docker system info | grep -q "Username"; then
        warn "未检测到 Docker 登录状态，正在尝试登录 ${REGISTRY}..."
        echo "请使用 GitHub Personal Access Token 登录。"
        echo "登录命令: docker login ${REGISTRY} -u ${USERNAME}"
        docker login "${REGISTRY}"
    fi

    docker push "${FULL_IMAGE_TAG}"
    info "推送成功: ${FULL_IMAGE_TAG}"
fi

# ------------------------------------------------------------------------------
# 运行示例
# ------------------------------------------------------------------------------
info "---------------------------------------------------"
info "构建完成！你可以使用以下命令运行容器："
info ""
info "  # 查看帮助信息"
info "  docker run --rm ${FULL_IMAGE_TAG} --help"
info ""
info "  # 运行 TTS 推理（输出文件为 ./output/output.wav）"
info "  mkdir -p models output"
info "  docker run --rm \\"
info "    -v \$(pwd)/models:/models:ro \\"
info "    -v \$(pwd)/output:/data \\"
info "    ${FULL_IMAGE_TAG} \\"
info "    --model /models/qwen3_fp32 \\"
info "    --text \"你好，世界！\""
info "---------------------------------------------------"