# -*- coding: UTF-8 -*-

# @author MuXiangyu, Chant Mee
# QAQ

import os
import fnmatch

TAB_NUM = 8
VALID_TYPE = ['*.c', '*.cpp', '*.py', '*.java', '*.cs', '*.html']

files = []
for root, dirnames, filenames in os.walk(os.path.expanduser('./')):
  for extension in VALID_TYPE:
    for filename in fnmatch.filter(filenames, extension):
      files.append(os.path.join(root,filename))

line_number = []
for file in files:
    with open(file, "r", encoding='ISO-8859-1') as f:
        content = f.read()
        f.close()
        content = content.split('\n')
        line_number.append([file, len(content)])

def length(s):
    cnt = 0
    for ch in s:
        if '\u4e00' <= ch <= '\u9fa5':
            cnt = cnt + 2
        else:
            cnt = cnt + 1
    return cnt

max_len = 0
for line in line_number:
    max_len = max(max_len, length(line[0]))
max_len = max_len // TAB_NUM + (0 if max_len % TAB_NUM != 0 else 1) + 1

print('File Name', end='')
for _ in range(max_len - 1):
    print('\t', end='')
print('Number of lines')

cnt = 0
for line in line_number:
    print(line[0], end="")
    for _ in range(max_len - length(line[0]) // TAB_NUM):
        print('\t', end='')
    print(line[1])
    cnt = cnt + line[1]

print('\ntotal', end='')
for _ in range(max_len - length("total") // TAB_NUM):
    print('\t', end='')
print(cnt)