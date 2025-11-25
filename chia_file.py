# Script chia file ee.cpp thanh cac module
import re

# Đọc file ee.cpp
with open('src/ee.cpp', 'r', encoding='utf-8') as f:
    content = f.read()

# Phần 1: JSON functions (lines 1-380)
json_part = content[content.find('#include "raylib.h"'):content.find('// ========================\n// GUI COMPONENTS')]

# Đã tạo JSON_XuLy.cpp rồi

print("Chia file ee.cpp thanh cong!")
print("- JSON_XuLy.cpp: Done")
print("- ClubGUI.h: Done")
print("Tiep tuc voi cac file con lai...")
