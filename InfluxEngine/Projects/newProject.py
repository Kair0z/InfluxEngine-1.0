import tkinter as tk
from tkinter import ttk
import sys
import argparse
from pathlib import Path

SAMPLES_DIR = Path(sys.argv[0]).parent
EXCLUDE_EXT = ['.user']
TEMPLATE_NAME = 'ProjectTemplate'

def create_project(name):
    # Source and destination directories.
    src_dir = SAMPLES_DIR / TEMPLATE_NAME
    dst_dir = SAMPLES_DIR / name

    print(f'Creating project "{name}":')

    # Check that destination does not exist.
    if dst_dir.exists():
        print(f'Project "{name}" already exists!')
        return False

    # Create destination folder.
    dst_dir.mkdir()

    # Copy project template.
    for src_file in filter(lambda f: not f.suffix in EXCLUDE_EXT, src_dir.iterdir()):
        dst_file = dst_dir / (src_file.name.replace(TEMPLATE_NAME, name))

        print(f'Writing {dst_file}.')

        # Replace all occurrences 'ProjectTemplate' with new project name.
        content = src_file.read_text()
        content = content.replace(TEMPLATE_NAME, name)
        dst_file.write_text(content)

    return True

root = tk.Tk()



#label:
label = ttk.Label(root, text = "Enter Project Name")
label.grid(column = 0, row = 0)

#Input Textbox:
projectName = tk.StringVar()
nameEntry = ttk.Entry(root, width = 15, textvariable = projectName)
nameEntry.grid(column = 0, row = 1)

def OnClick():
    name = projectName.get()
    if not name:
        return

    success = create_project(name)
    root.destroy

#Button:
button = ttk.Button(root, text = "Generate", command = OnClick)
button.grid(column = 0, row = 2)

root.mainloop()


