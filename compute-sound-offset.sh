#!/bin/bash

set -e

if [ $# -ne 3 ]; then
    echo "usage: $0 good_sound.wav bad_sound.wav duration_limit"
    echo "where"
    echo "  good_sound.wav - taken from audio recorder"
    echo "  bad_sound.wav - taken from camera"
    echo "  duration_limit - max seconds to analyze, 0 means no limit"
    exit 1
fi

GOOD_SOUND="$1"
BAD_SOUND="$2"
DURATION_LIMIT="$3"

downsample_and_cut ()
{
    if [ "${DURATION_LIMIT}" = 0 ]; then
        duration_limit_args=""
    else
        duration_limit_args="-t ${DURATION_LIMIT}"
    fi
    ffmpeg -y -hide_banner -loglevel error -i "$1" -ab 8 -ar 8000 ${duration_limit_args} -map 0:a -ac 1 -vn "$2"
}

TEMP_DIR=$(mktemp -d /var/tmp/$(basename $0).XXXXXXXXXX)
SCRIPT_DIR="$(dirname $(realpath $0))"
COMPUTE_OFFSET="${SCRIPT_DIR}/compute_sound_offset"

TMP_GOOD_SOUND="${TEMP_DIR}/$(basename ${GOOD_SOUND}).wav"
TMP_BAD_SOUND="${TEMP_DIR}/$(basename ${BAD_SOUND}).wav"
downsample_and_cut "${GOOD_SOUND}" "${TMP_GOOD_SOUND}"
downsample_and_cut "${BAD_SOUND}" "${TMP_BAD_SOUND}"

OFFSET=$("${COMPUTE_OFFSET}" "${TMP_GOOD_SOUND}" "${TMP_BAD_SOUND}")
echo "${OFFSET}"

rm -rf "${TEMP_DIR}"
