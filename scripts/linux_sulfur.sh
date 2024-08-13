COMP=gcc
COMP_ARGS='-fno-builtin-printf -Wall -Wextra -Werror -Wno-format  -Wno-cast-function-type -Wno-unused-variable -Wno-unused-parameter'
LD_ARGS='-lm'

OUTPUT=build/sulfur
TMP=tmp/

mkdir $TMP

o_count=0




