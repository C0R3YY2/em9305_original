
; Assembler directives for eia extensions in this design
.set apex_crc_extension_present,1
.extInstruction crc32, 7, 0x00, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc32h, 7, 0x01, SUFFIX_COND, SYNTAX_3OP
.extInstruction bswp16, 7, 0x0d, FLAGS_NONE, SYNTAX_2OP
.extInstruction bswp8, 7, 0x0e, FLAGS_NONE, SYNTAX_2OP
.extInstruction crc32b, 7, 0x02, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc16, 7, 0x04, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc16h, 7, 0x05, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc16b, 7, 0x06, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc24, 7, 0x08, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc24h, 7, 0x09, SUFFIX_COND, SYNTAX_3OP
.extInstruction crc24b, 7, 0x0a, SUFFIX_COND, SYNTAX_3OP
.extInstruction bswp32, 7, 0x0c, FLAGS_NONE, SYNTAX_2OP
 .set apex_jli_rebase_extension_present,1
.extInstruction jliarbs, 7, 0x10, SUFFIX_COND, SYNTAX_3OP
 .set apex_log2_extension_present,1
.set apex_log2_extension_present,1
.extInstruction log2p1,7,0,FLAGS_NONE,SYNTAX_2OP
.extInstruction log2p0,7,1,FLAGS_NONE,SYNTAX_2OP
.set apex_com_arc_hardware_floating_point_unit_present,1
.extAuxRegister fpu_build,0xc8,r
.extAuxRegister fpu_ctrl,0x300,r|w
.extAuxRegister fpu_status,0x301,r|w
.extInstruction fsmul,6,0,SUFFIX_COND,SYNTAX_3OP
.extInstruction fsadd,6,1,SUFFIX_COND,SYNTAX_3OP
.extInstruction fssub,6,2,SUFFIX_COND,SYNTAX_3OP
.extInstruction fcvt32,6,8,SUFFIX_COND,SYNTAX_3OP
.extInstruction fsdiv,6,7,SUFFIX_COND,SYNTAX_3OP
.extInstruction fscmp,6,3,SUFFIX_COND|SUFFIX_FLAG,SYNTAX_3OP
.extInstruction fscmpf,6,4,SUFFIX_COND|SUFFIX_FLAG,SYNTAX_3OP
.extInstruction fssqrt,6,0,FLAGS_NONE,SYNTAX_2OP

