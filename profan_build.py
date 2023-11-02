import os

CC      = "gcc"
LD      = "ld"

OUTPUT  = "build/sulfur"

CFLAGS  = "-ffreestanding -fno-exceptions -fno-stack-protector -m32 -I ./build/profan -D ONE_FILE -D __profanOS__"
LDFLAGS = "-T build/profan/_link.ld"

OBJDIR  = "build/profan_objects"

SRC_DIRS = ["src", "src/bytecode_maker", "sulfur_libs/blt_libs", "sulfur_libs/std_libs", "sulfur_libs/std_libs/graphic"]

ENDNOTE = """
    The  file  "sulfur.bin"  was  successfully
     generated from the output folder "build/"
    please move it to the profanOS file system!
"""

def execute_command(cmd, display = True):
    if display: print(cmd)
    rcode = os.system(cmd)
    if rcode == 0: return
    print(f"Command failed with exit code {rcode}")
    exit(rcode if rcode < 256 else 1)

def compile_file(src, dir):
    obj = os.path.join(OBJDIR, f"{os.path.splitext(src)[0]}.o")
    cmd = f"{CC} -c {os.path.join(dir, src)} -o {obj} {CFLAGS}"
    print(f"CC {src}")
    execute_command(cmd, display = False)
    return obj

def link_files(entry, objs, output = OUTPUT):
    execute_command(f"{LD} {LDFLAGS} -o {output}.pe {entry} {' '.join(objs)}")
    execute_command(f"objcopy -O binary {output}.pe {output}.bin")

def main():
    execute_command(f"mkdir -p {OBJDIR}")

    objs = []

    for dir in SRC_DIRS:
        objs.extend(
            compile_file(src, dir)
            for src in [e for e in os.listdir(dir) if e.endswith(".c")]
        )

    objs.append(compile_file("main.c", "."))

    entry = compile_file("_entry.c", "build/profan")
    link_files(entry, objs)

    execute_command(f"rm {OUTPUT}.pe")

    print(ENDNOTE)

if __name__ == "__main__":
    main()
