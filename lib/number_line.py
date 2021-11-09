import os

files = os.listdir("./")
line_number = []
for file in files:
    with open(file, "r", encoding='ISO-8859-1') as f:
        content = f.read()
        f.close()
        content = content.split('\n')
        line_number.append([file, len(content)])

max_len = 0
for line in line_number:
    max_len = max(max_len, len(line[0]))
max_len = max_len // 8 + 2

print('File Name', end='')
for _ in range(max_len - 1):
    print('\t', end='')
print('Number of lines')

cnt = 0
for line in line_number:
    print(line[0], end="")
    for _ in range(max_len - len(line[0]) // 8):
        print('\t', end='')
    print(line[1])
    cnt = cnt + line[1]

print('\ntotal', end='')
for _ in range(max_len - len("total") // 8):
    print('\t', end='')
print(cnt)