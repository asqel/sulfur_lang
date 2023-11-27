#ifndef BYTECODE_MAKER_OP_CODES_H
#define BYTECODE_MAKER_OP_CODES_H

#define MAGICAL_NUMBER0 '.'
#define MAGICAL_NUMBER1 'S'
#define MAGICAL_NUMBER2 'u'
#define MAGICAL_NUMBER3 '.'

#define OUNT_MAGIC_NUM 'I'
#define STRING_MAGIC_NUM 'S'
#define FLOAP_MAGIC_NUM 'F'

#define OPC_END 0x00
#define OPC_PASS 0x01
#define OPC_JMP 0x02
#define OPC_JMP_AT 0x03
#define OPC_JMP_AT_IF 0x04
#define OPC_JMP_AT_IFN 0x05
#define OPC_CALL 0x06
#define OPC_RET 0x07
#define OPC_PUSH 0x08
#define OPC_PUSH_VAR 0x09
#define OPC_POP 0x0A
#define OPC_CALL_FUNC 0x0B
#define OPC_RET_FUNC 0x0C
#define OPC_DEF_FUNC 0x0D
#define OPC_DUP 0x0E
#define OPC_SWAP 0x0F

#define SIZE_END 1
#define SIZE_PASS 1
#define SIZE_JMP 9
#define SIZE_JMP_AT 1
#define SIZE_JMP_AT_IF 1
#define SIZE_JMP_AT_IFN 1
#define SIZE_CALL 1
#define SIZE_RET 1
#define SIZE_PUSH 9
#define SIZE_PUSH_VAR 1
#define SIZE_POP 1
#define SIZE_CALL_FUNC 1
#define SIZE_RET_FUNC 1
#define SIZE_DEF_FUNC 10

#endif