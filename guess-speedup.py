#!/bin/env python3

import logging
import math
import os
import shutil
from subprocess import getoutput
import sys

if len(sys.argv) < 3:
    print('syntax: {} sound_with_unknown_speed.wav sound_to_sync_with.wav'.format(sys.argv[0]))
    print('This script tries to guess how the file should sped up, so it can be synced with file')
    sys.exit(1)

TEMP_DIR = getoutput("mktemp -d /var/tmp/{0}.XXXXXXXXXX".format(os.path.basename(sys.argv[0])))
SCRIPT_DIR = os.path.dirname(sys.argv[0])
COMPUTE_SOUND_OFFSET_SH = "%s/compute-sound-offset.sh" % SCRIPT_DIR
EPS = 0.0001
MIN_DURATION = 10.0
FFMPEG = 'ffmpeg -y -hide_banner -loglevel error'

def remove_temp_dir():
    shutil.rmtree(path=TEMP_DIR, ignore_errors=True)

def create_temp_wav_file():
    os.makedirs(TEMP_DIR, exist_ok=True)
    return getoutput("mktemp {0}/tmp.XXXXXXXXXX".format(TEMP_DIR)) + '.wav'

def compute_shift(filename_a, filename_b):
    logging.debug('compute_shift %s %s', filename_a, filename_b)
    shift = getoutput("%s %s %s 0 2>>/dev/null" % (COMPUTE_SOUND_OFFSET_SH, filename_a, filename_b))
    logging.debug('shift %s', shift)
    return float(shift)

def get_duration(filename):
    duration = float(getoutput('ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 %s 2>>/dev/null' % filename))
    return duration

def get_half_of_duration(filename):
    duration = get_duration(filename)
    return duration / 2.

def divide(filename):
    return left(filename), right(filename)

def left(filename):
    half = get_half_of_duration(filename)
    left_filename = create_temp_wav_file()
    os.system('{0} -i {1} -t {2} {3}'.format(FFMPEG, filename, half, left_filename))
    return left_filename

def right(filename):
    half = get_half_of_duration(filename)
    right_filename = create_temp_wav_file()
    os.system('{0} -ss {1} -i {2} {3}'.format(FFMPEG, half, filename, right_filename))
    return right_filename

def eq(x, y):
    return abs(x - y) < EPS

def guess_duration(filename_with_unknown_duration, filename_b, original_duration_a, duration_b):
    filename_a = filename_with_unknown_duration
    duration_a = original_duration_a
    max_duration = max(duration_a, duration_b)
    min_duration = min(duration_a, duration_b)
    if min_duration <= MIN_DURATION:
        return duration_a
    left_a, right_a = divide(filename_a)
    left_b, right_b = divide(filename_b)
    shift_left = compute_shift(left_a, left_b)
    shift_right = compute_shift(right_a, right_b)
    prev_shift_left = math.inf
    prev_shift_right = math.inf
    attempt = 1
    while not eq(shift_left, shift_right) and min_duration >= MIN_DURATION and not eq(shift_left, prev_shift_left) and not eq(shift_right, prev_shift_right):
        left_a = left(left_a)
        right_a = right(right_a)
        left_b = left(left_b)
        right_b = right(right_b)
        prev_shift_left = shift_left
        prev_shift_right = shift_right
        shift_left = compute_shift(left_a, left_b)
        shift_right = compute_shift(right_a, right_b)
        min_duration /= 2
        guessed_duration_a = duration_b - shift_left - shift_right
        logging.debug('attempt %d, shift_left %f, shift_right %f, min_duration %f, max_duration %f', attempt, shift_left, shift_right, min_duration, max_duration)
        attempt += 1
    return guessed_duration_a

logging.basicConfig(level=logging.DEBUG)
remove_temp_dir()
good_sound = sys.argv[1]
bad_sound = sys.argv[2]

original_duration_a = get_duration(good_sound)
duration_b = get_duration(bad_sound)
guessed_duration_of_good_sound = guess_duration(good_sound, bad_sound, original_duration_a, duration_b)
speedup = guessed_duration_of_good_sound / duration_b

remove_temp_dir()
print(speedup)
