# -*- coding: UTF-8 -*-

# @author MuXiangyu, Chant Mee
# QAQ

import os

res = []
directory = r"./"

for root, dirs, files in os.walk(directory, topdown=False):
    for name in dirs:
        dir_path = os.path.join(root, name)
        dir_path = dir_path.split("/")
        dir_path = dir_path[1:]
        if len(res) == 0 or len(dir_path) != len(res[-1]):
            res.append(dir_path)
        else:
            for i in range(len(res[-1])):
                if res[-1][i] != dir_path[i]:
                    res.append(dir_path)
                    break

def escape(s: str):
   s = s.replace("\\", '\\\\')
   s = s.replace(" ", '\\s')
   s = s.replace("\n", '\\n')
   s = s.replace("\t", '\\t')
   return s

for path in res:
    for name in path:
        name = escape(name)
        print("mkdir " + name)
        print("cd " + name)
    for _ in range(len(path)):
        print("cdl")

def is_type_valid(dir):
    valid_type = ["md", "txt", "cpp", "c", "py", "m"]
    dir = dir.split(".")
    for vt in valid_type:
        if dir[-1] == vt:
            return True
    return False

def get_content(dir):
    if is_type_valid(dir) is not True:
        return "\s"
    with open(dir, "r", encoding='ISO-8859-1') as f:
        content = f.read()
        f.close()
        return content

res = []
for root, dirs, files in os.walk(directory, topdown=False):
    for name in files:
        file_path = os.path.join(root, name)
        file_path = file_path.split("/")
        file_path = file_path[1:]
        if len(res) == 0 or len(file_path) != len(res[-1]):
            res.append(file_path)
        else:
            for i in range(len(res[-1])):
                if res[-1][i] != file_path[i]:
                    res.append(file_path)
                    break

for path in res:
    no_escape_path = "./"
    for i in range(len(path)):
        no_escape_path = no_escape_path + path[i]
        if i < len(path) - 1:
            no_escape_path = no_escape_path + "/"
        path[i] = escape(path[i])
        if i < len(path) - 1:
            print("cd " + path[i])
        else:
            print("touch " + path[i])
            content = get_content(no_escape_path)
            content = escape(content)
            print("update_content " + path[i] + " " + content)
    for _ in range(len(path)):
        print('cdl')
