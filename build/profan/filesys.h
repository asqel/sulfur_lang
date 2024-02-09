#ifndef FILESYS_LIB_ID
#define FILESYS_LIB_ID 1010
#define DEVIO_LIB_ID   1015
#define FMOPEN_LIB_ID  1016

#include <type.h>

#ifndef NULL_SID
#define NULL_SID ((sid_t) {0, 0})
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 0xFFFFFFFF
#endif

#ifndef META_MAXLEN
#define META_MAXLEN 64
#endif

#ifndef ROOT_SID
#define ROOT_SID ((sid_t) {1, 0})
#endif

#ifndef IS_NULL_SID
#define IS_NULL_SID(sid) (sid.device == 0 && sid.sector == 0)
#endif

#ifndef IS_SAME_SID
#define IS_SAME_SID(sid1, sid2) (sid1.device == sid2.device && sid1.sector == sid2.sector)
#endif

#define DEVIO_STDOUT 0
#define DEVIO_STDERR 1
#define DEVIO_BUFFER 2

#define fu_get_file_size(sid) (c_fs_cnt_get_size(c_fs_get_main(), sid))
#define fu_set_file_size(sid, size) (c_fs_cnt_set_size(c_fs_get_main(), sid, size))

#define fu_fctf_write(sid, buf, offset, size) (fu_fctf_rw(sid, buf, offset, size, 0))
#define fu_fctf_read(sid, buf, offset, size) (fu_fctf_rw(sid, buf, offset, size, 1))
#define fu_fctf_flush(sid) (fu_fctf_rw(sid, NULL, 0, 0, 2))


#define get_func_addr ((uint32_t (*)(uint32_t, uint32_t)) *(uint32_t *) 0x1ffffb)

#ifndef FILESYS_LIB_C

#define fu_sep_path ((void (*)(char *, char **, char **)) get_func_addr(FILESYS_LIB_ID, 2))
#define fu_is_dir ((int (*)(sid_t)) get_func_addr(FILESYS_LIB_ID, 3))
#define fu_get_dir_content ((int (*)(sid_t, sid_t **, char ***)) get_func_addr(FILESYS_LIB_ID, 4))
#define fu_add_element_to_dir ((int (*)(sid_t, sid_t, char *)) get_func_addr(FILESYS_LIB_ID, 5))
#define fu_remove_element_from_dir ((int (*)(sid_t, sid_t)) get_func_addr(FILESYS_LIB_ID, 6))
#define fu_dir_create ((sid_t (*)(int, char *)) get_func_addr(FILESYS_LIB_ID, 7))
#define fu_is_file ((int (*)(sid_t)) get_func_addr(FILESYS_LIB_ID, 8))
#define fu_file_create ((sid_t (*)(int, char *)) get_func_addr(FILESYS_LIB_ID, 9))
#define fu_file_read ((int (*)(sid_t, void *, uint32_t, uint32_t)) get_func_addr(FILESYS_LIB_ID, 10))
#define fu_file_write ((int (*)(sid_t, void *, uint32_t, uint32_t)) get_func_addr(FILESYS_LIB_ID, 11))
#define fu_is_fctf ((int (*)(sid_t)) get_func_addr(FILESYS_LIB_ID, 12))
#define fu_fctf_create ((sid_t (*)(int, char *, int (*)(void *, uint32_t, uint32_t, uint8_t))) get_func_addr(FILESYS_LIB_ID, 13))
#define fu_fctf_rw ((int (*)(sid_t, void *, uint32_t, uint32_t, uint8_t)) get_func_addr(FILESYS_LIB_ID, 14))
#define fu_fctf_get_addr ((uint32_t (*)(sid_t)) get_func_addr(FILESYS_LIB_ID, 15))

#define fu_path_to_sid ((sid_t (*)(sid_t, char *)) get_func_addr(FILESYS_LIB_ID, 17))
#define fu_simplify_path ((void (*)(char *)) get_func_addr(FILESYS_LIB_ID, 18))
#define fu_get_vdisk_info ((uint32_t *(*)(void)) get_func_addr(FILESYS_LIB_ID, 19))

#endif
#ifndef FMOPEN_LIB_C

#define fm_open ((int (*)(char *)) get_func_addr(FMOPEN_LIB_ID, 2))
#define fm_reopen ((int (*)(int, char *)) get_func_addr(FMOPEN_LIB_ID, 3))
#define fm_close ((int (*)(int)) get_func_addr(FMOPEN_LIB_ID, 4))
#define fm_read ((int (*)(int, void *, uint32_t)) get_func_addr(FMOPEN_LIB_ID, 5))
#define fm_write ((int (*)(int, void *, uint32_t)) get_func_addr(FMOPEN_LIB_ID, 6))
#define fm_lseek ((int (*)(int, int, int)) get_func_addr(FMOPEN_LIB_ID, 7))
#define fm_tell ((int (*)(int)) get_func_addr(FMOPEN_LIB_ID, 8))
#define fm_dup ((int (*)(int)) get_func_addr(FMOPEN_LIB_ID, 9))
#define fm_dup2 ((int (*)(int, int)) get_func_addr(FMOPEN_LIB_ID, 10))
#define fm_pipe ((int (*)(int[2])) get_func_addr(FMOPEN_LIB_ID, 11))
#define fm_isfctf ((int (*)(int)) get_func_addr(FMOPEN_LIB_ID, 12))
#define fm_clean ((void (*)(void)) get_func_addr(FMOPEN_LIB_ID, 13))
#define fm_resol012 ((int (*)(int, int)) get_func_addr(FMOPEN_LIB_ID, 15))

#endif
#endif
