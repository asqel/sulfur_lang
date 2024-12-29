import os, sys

profan_path = "../profanOS"
if sys.argv[1:]:
    profan_path = sys.argv[1]
if not os.path.exists(profan_path):
    print(f"path {profan_path} does not exist")
    exit(1)

CC      = "gcc"
LD      = "ld"

OUTPUT  = "build/sulfur.elf"

CFLAGS  = f"-ffreestanding -fno-exceptions -fno-stack-protector -m32 -I {profan_path}/include/addons -I {profan_path}/include/zlibs -D ONE_FILE=1 -D __profanOS__ -nostdinc -O3 -fno-omit-frame-pointer"
LDFLAGS = f"-nostdlib -m elf_i386 -L {profan_path}/out/zlibs -T {profan_path}/tools/link_elf.ld -lc -lm"

OBJDIR  = "build/profan_objects"

SRC_DIRS = ["src", "sulfur_libs/blt_libs", "sulfur_libs/std_libs", "sulfur_libs/std_libs/graphic"]

ENDNOTE = """
    The  file  "sulfur.elf"  was  successfully
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
    execute_command(f"{LD} {LDFLAGS} -o {output} {entry} {' '.join(objs)}")

def main():
    execute_command(f"mkdir -p {OBJDIR}")

    objs = []

    for dir in SRC_DIRS:
        objs.extend(
            compile_file(src, dir)
            for src in [e for e in os.listdir(dir) if e.endswith(".c")]
        )

    objs.append(compile_file("main.c", "."))

    entry = compile_file("entry_elf.c", f"{profan_path}/tools")
    link_files(entry, objs)

    print(ENDNOTE)

if __name__ == "__main__":
    main()
