#!/usr/bin/env bash
set -euo pipefail

if [[ ${1:-} == "" ]]; then
  echo "Usage: $0 <Platform> [Configuration]" >&2
  echo "Example: $0 Win64 Development" >&2
  exit 1
fi

PLATFORM="$1"
CONFIGURATION="${2:-Development}"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PROJECT_FILE="${PROJECT_ROOT}/Project_Neon.uproject"

: "${UE_ROOT:?UE_ROOT must point to the Unreal Engine installation root}"
RUN_UAT="${UE_ROOT}/Engine/Build/BatchFiles/RunUAT.sh"

if [[ ! -f "${RUN_UAT}" ]]; then
  echo "Unable to find RunUAT at ${RUN_UAT}" >&2
  exit 2
fi

if [[ ! -f "${PROJECT_FILE}" ]]; then
  echo "Unable to find Unreal project file at ${PROJECT_FILE}" >&2
  echo "Ensure the .uproject lives at the repository root or adjust PROJECT_FILE." >&2
  exit 3
fi

OUTPUT_DIR="${PROJECT_ROOT}/Saved/Builds/${PLATFORM}/${CONFIGURATION}"
mkdir -p "${OUTPUT_DIR}"

"${RUN_UAT}" BuildCookRun \
  -project="${PROJECT_FILE}" \
  -noP4 \
  -platform="${PLATFORM}" \
  -clientconfig="${CONFIGURATION}" \
  -serverconfig="${CONFIGURATION}" \
  -cook \
  -build \
  -stage \
  -pak \
  -archive \
  -archivedirectory="${OUTPUT_DIR}" \
  "$@"
