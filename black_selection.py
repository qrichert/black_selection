#!/usr/bin/env python3

import os
import re
import sys
import tempfile

# Get args.
if len(sys.argv) < 5:
    print(
        "Error: Missing arguments.\n\n    python3 black_selection.py "
        "/path/to/black input_file.py start_line end_line [line_length]\n"
    )
    exit(1)

black: str = sys.argv[1]
input_file: str = sys.argv[2]
line_start: int = int(sys.argv[3]) - 1
line_end: int = int(sys.argv[4])
line_length: int = -1
if len(sys.argv) >= 6:
    line_length = int(sys.argv[5])

# Get selected code (line range).
with open(input_file, "r") as f:
    src_content: list = [line for line in f]
    selection: list = src_content[line_start:line_end]
    if not selection:
        exit(0)

# Save leading newlines, and remove them from selection (start with code).
leading_newlines: str = ""
while selection[0] == "\n":
    leading_newlines += selection[0]
    del selection[0]

# Save trailing newlines, and remove them from selection (end with code).
trailing_newlines: str = ""
while selection[-1] == "\n":
    trailing_newlines += selection[-1]
    del selection[-1]

# If selection is indented, make it valid Python by putting it into blocks.
leading_white_space: str = re.search(r"^(\s*)", selection[0]).group(1)
indentation_level: int = 0
if leading_white_space:
    indentation_char: str = ""
    leading_white_space_length: int = len(leading_white_space)
    if " " in leading_white_space:  # 4 spaces
        indentation_level = leading_white_space_length // 4
        indentation_char = "    "
    elif "\t" in leading_white_space:  # tabs
        indentation_level = leading_white_space_length
        indentation_char = "\t"
    for i in range(indentation_level):
        placeholder_block: str = (indentation_char * i) + "if True:\n"
        selection.insert(i, placeholder_block)

# Write selection to tmp file.
tmp_dir = tempfile.TemporaryDirectory()
tmp_file_name = os.path.join(tmp_dir.name, "black_selection__tmp")
with open(tmp_file_name, "w") as f:
    f.writelines(selection)

# Run black on tmp file.
args: list = []
if line_length > 0:
    args.append(f"--line-length {line_length}")
os.system(f"{black} {' '.join(args)} {tmp_file_name}")

# Replace selected lines with reformatted lines.
with open(tmp_file_name, "r") as f:
    lines: list = f.readlines()
    # Trim blackened code.
    while lines[0] == "\n":
        del lines[0]
    while lines[-1] == "\n":
        del lines[-1]
    # Remove placeholder indent blocks.
    lines = lines[indentation_level:]
    # Trim blackened code again (for lines added after indent block).
    while lines[0] == "\n":
        del lines[0]
    # Restore saved newlines.
    lines.insert(0, leading_newlines)
    lines.append(trailing_newlines)
    # Remove selection from original.
    del src_content[line_start:line_end]
    # Insert blackened selection into original.
    src_content.insert(line_start, "".join(lines))

# Update input file.
with open(input_file, "w") as f:
    f.writelines(src_content)
