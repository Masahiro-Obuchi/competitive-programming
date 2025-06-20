#!/bin/bash

# コンテスト名と番号をチェック
if [ $# -ne 2 ]; then
    echo "Usage: $0 <contest_type> <contest_number>"
    echo "Example: $0 ABC 410"
    echo "         $0 AGC 070"
    echo "         $0 ARC 180"
    echo "         $0 AHC 040"
    echo "         $0 CF 2000A"
    exit 1
fi

CONTEST_TYPE=$1
CONTEST_NUMBER=$2

# ディレクトリ名を生成
CONTEST_NAME="${CONTEST_TYPE}${CONTEST_NUMBER}"
DIR_NAME=${CONTEST_TYPE}/"${CONTEST_NAME}"
TARGET_DIR="solved_problems/${DIR_NAME}"

# すでにディレクトリが存在する場合は確認
if [ -d "$TARGET_DIR" ]; then
    echo "Directory $TARGET_DIR already exists. Do you want to continue? [y/N]"
    read -r response
    if [[ ! "$response" =~ ^[Yy]$ ]]; then
        echo "Aborted."
        exit 1
    fi
fi

# ディレクトリ作成
mkdir -p "$TARGET_DIR"

# ファイル名のプレフィックスを生成（小文字）
PREFIX=$(echo "${CONTEST_TYPE}${CONTEST_NUMBER}")

PROBLEMS=("a" "b" "c" "d" "e" "f" "g" "h")
for problem in "${PROBLEMS[@]}"; do
    cp_file="solution.cpp"
    target_file="${TARGET_DIR}/${problem}.cpp"
    
    if [ -f "$cp_file" ]; then
        cp "$cp_file" "$target_file"
        echo "Created: $target_file"
    else
        echo "Warning: Template file $cp_file not found"
    fi
done

echo ""
echo "Successfully created contest directory: $TARGET_DIR"
echo "Files created:"
echo "  - 8 solution files (${PREFIX}_a.cpp to ${PREFIX}_h.cpp)"
echo "  - CMakeLists.txt"
echo "  - build.sh"

