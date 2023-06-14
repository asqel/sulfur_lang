import os

CC      = "gcc"
LD      = "ld"

OUTPUT  = "sulfur"

CFLAGS  = "-ffreestanding -fno-exceptions -fno-stack-protector -m32 -I ./profan_libs -D ONE_FILE -D __profanOS__"
LDFLAGS = "-T link.ld"

OBJDIR  = "build"

def execute_command(cmd):
    print(cmd)
    rcode = os.system(cmd)
    if rcode == 0: return
    print(f"Command failed with exit code {rcode}")
    exit(rcode if rcode < 256 else 1)

def compile_file(src, dir):
    obj = os.path.join(OBJDIR, f"{os.path.splitext(src)[0]}.o")
    cmd = f"{CC} -c {os.path.join(dir, src)} -o {obj} {CFLAGS}"
    execute_command(cmd)
    return obj

def link_files(entry, objs, output = OUTPUT):
    execute_command(f"{LD} {LDFLAGS} -o {output}.pe {entry} {' '.join(objs)}")
    execute_command(f"objcopy -O binary {output}.pe {output}.bin")

def main():
    execute_command(f"mkdir -p {OBJDIR}")

    objs = []

    for dir in ["src", "sulfur_libs/blt_libs", "sulfur_libs/std_libs"]:
        objs.extend(
            compile_file(src, dir)
            for src in [e for e in os.listdir(dir) if e.endswith(".c")]
        )

    objs.append(compile_file("main.c", "."))
    
    entry = compile_file("entry.c", ".")
    link_files(entry, objs)

    execute_command("rm *.pe")

if __name__ == "__main__":
    main()
