################################################################################
###
### @file       common/9305/NVM/linker.cmd
###
### @project    T9305
###
### @brief      Linker script for the NVM.
###
### @classification  Internal Use
###
################################################################################
###
################################################################################
###
### @copyright Copyright (c) 2022, EM Microelectronic
### @cond
###
### All rights reserved.
###
### Redistribution and use in source and binary forms, with or without
### modification, are permitted provided that the following conditions are met:
### 1. Redistributions of source code must retain the above copyright notice,
### this list of conditions and the following disclaimer.
### 2. Redistributions in binary form must reproduce the above copyright notice,
### this list of conditions and the following disclaimer in the documentation
### and/or other materials provided with the distribution.
###
################################################################################
###
### THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
### AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
### IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
### ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
### LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
### CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
### SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
### INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
### CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
### ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
### POSSIBILITY OF SUCH DAMAGE.
### @endcond
################################################################################

INITDATA !data, !bss, ~.rom_nvm_const, ~.nvm_const, ~.nvm_info_const, ~.rodata


#
# Get memory usage of the ROM.
#

# For ROM v1 (MEMORY_ROM_* not defined): PRAM=Hardcoded / NPRAM=Discarded
_MEMORY_ROM_PRAM_USED_END    = DEFINED(MEMORY_ROM_PRAM_USED_END)    ? MEMORY_ROM_PRAM_USED_END    : 0x0080_1138;
_MEMORY_ROM_NPRAM_USED_START = DEFINED(MEMORY_ROM_NPRAM_USED_START) ? MEMORY_ROM_NPRAM_USED_START : 0x0081_0000;

# Compute used size of PRAM and NPRAM by the ROM.
ROM_USED_PRAM_SIZE  = _MEMORY_ROM_PRAM_USED_END - 0x0080_1000;
ROM_USED_NPRAM_SIZE = 0x0081_0000 - _MEMORY_ROM_NPRAM_USED_START;


#
# Compute require NPRAM for static variable allocation.
#

# Round up the size of the NPRAM sections to be a multiple of 4 bytes.
MEMORY_NOINIT_SIZE          = ((SIZEOF(.noinit)         + 3) >> 2) << 2;
MEMORY_CONFIG_SIZE          = ((SIZEOF(.config)         + 3) >> 2) << 2;
MEMORY_NONPERSISTENT_SIZE   = ((SIZEOF(.non_persistent) + 3) >> 2) << 2;
# Compute the start of used NPRAM.
MEMORY_NPRAM_USED_START     = _MEMORY_ROM_NPRAM_USED_START  -
                              MEMORY_NOINIT_SIZE            -
                              MEMORY_CONFIG_SIZE            -
                              MEMORY_NONPERSISTENT_SIZE;
MEMORY_NVM_NPRAM_USED_START = MEMORY_NPRAM_USED_START;


#
# Defines for the stack.
#

# Place the stack just before the start of used NPRAM.
_STACKSIZE = DEFINED(STACKSIZE) ? STACKSIZE : 1024;
_fstack = MEMORY_NPRAM_USED_START - _STACKSIZE;
_estack = MEMORY_NPRAM_USED_START;


#
# Memory regions.
#

MEMORY {
    IRAM    (rwx) : ORIGIN = 0x0018_0000, LENGTH = 32K     # DRAM6 + DRAM5 remmaped into ICCM
    IRAM1   (rwx) : ORIGIN = 0x0018_0000, LENGTH = 16K     # Same physical memory as DRAM6 (remapped into ICCM)
    IRAM2   (rwx) : ORIGIN = 0x0018_4000, LENGTH = 16K     # Same physical memory as DRAM5 (remapped into ICCM)
    IRAM0   (rwx) : ORIGIN = 0x001F_F000, LENGTH = 4K      # JLI table

    NVM      (rx) : ORIGIN = 0x0030_0000, LENGTH = 512K    # NVM main area
    NVMINFO  (rx) : ORIGIN = 0x0040_0000, LENGTH = 32K     # NVM info area

    DRAM0    (rw) : ORIGIN = 0x0080_0000, LENGTH = 4K
    DRAM1    (rw) : ORIGIN = 0x0080_1000, LENGTH = 4K
    DRAM2    (rw) : ORIGIN = 0x0080_2000, LENGTH = 4K
    DRAM3    (rw) : ORIGIN = 0x0080_3000, LENGTH = 4K
    DRAM4    (rw) : ORIGIN = 0x0080_4000, LENGTH = 16K
    DRAM5    (rw) : ORIGIN = 0x0080_8000, LENGTH = 16K
    DRAM6    (rw) : ORIGIN = 0x0080_C000, LENGTH = 16K

    PRAM     (rw) : ORIGIN = 0x0080_1000, LENGTH = _fstack - 0x0080_1000
    POOL     (rw) : ORIGIN = 0x0080_1000, LENGTH = 60K
    NPRAM    (rw) : ORIGIN = _fstack,     LENGTH = 0x0081_0000 - _fstack
}

#
# Defines for the memory regions.
#

MEMORY_IRAM1_START    = ADDR(IRAM1);
MEMORY_IRAM1_END      = ADDR(IRAM1) + SIZEOF(IRAM1);

MEMORY_IRAM2_START    = ADDR(IRAM2);
MEMORY_IRAM2_END      = ADDR(IRAM2) + SIZEOF(IRAM2);

MEMORY_IRAM0_START    = ADDR(IRAM0);
MEMORY_IRAM0_END      = ADDR(IRAM0) + SIZEOF(IRAM0);

MEMORY_NVM_START      = ADDR(NVM);
MEMORY_NVM_END        = ADDR(NVM) + SIZEOF(NVM);

MEMORY_NVMINFO_START  = ADDR(NVMINFO);
MEMORY_NVMINFO_END    = ADDR(NVMINFO) + SIZEOF(NVMINFO);

MEMORY_DRAM0_START    = ADDR(DRAM0);
MEMORY_DRAM0_END      = ADDR(DRAM0) + SIZEOF(DRAM0);

MEMORY_DRAM1_START    = ADDR(DRAM1);
MEMORY_DRAM1_END      = ADDR(DRAM1) + SIZEOF(DRAM1);

MEMORY_DRAM2_START    = ADDR(DRAM2);
MEMORY_DRAM2_END      = ADDR(DRAM2) + SIZEOF(DRAM2);

MEMORY_DRAM3_START    = ADDR(DRAM3);
MEMORY_DRAM3_END      = ADDR(DRAM3) + SIZEOF(DRAM3);

MEMORY_DRAM4_START    = ADDR(DRAM4);
MEMORY_DRAM4_END      = ADDR(DRAM4) + SIZEOF(DRAM4);

MEMORY_DRAM5_START    = ADDR(DRAM5);
MEMORY_DRAM5_END      = ADDR(DRAM5) + SIZEOF(DRAM5);

MEMORY_DRAM6_START    = ADDR(DRAM6);
MEMORY_DRAM6_END      = ADDR(DRAM6) + SIZEOF(DRAM6);

MEMORY_PRAM_START     = ADDR(PRAM);
MEMORY_PRAM_END       = ADDR(PRAM) + SIZEOF(PRAM);

MEMORY_NPRAM_START    = ADDR(NPRAM);
MEMORY_NPRAM_END      = ADDR(NPRAM) + SIZEOF(NPRAM);

MEMORY_POOL_START     = ADDR(POOL);
MEMORY_POOL_END       = ADDR(POOL) + SIZEOF(POOL);


#
# Linker sections.
#

# define base addresses
NVMBL_ENTRY_ADDR = MEMORY_NVM_START;
NVM_ORIGIN = 0x0030_2000;
BL_ENTRY_ADDR = 0x0036_0000;
# overwite for special sections
NVM_ORIGIN = DEFINED(APP_IS_BOOTLOADER) ? BL_ENTRY_ADDR : NVM_ORIGIN;
NVM_ORIGIN = DEFINED(NVM_CUSTOM_ORIGIN) ? NVM_CUSTOM_ORIGIN : NVM_ORIGIN;

SECTIONS {
    # Persistent memory of the ROM.
    GROUP: {
        .rom_used_pram NOLOAD SIZE(ROM_USED_PRAM_SIZE): {}
    } > PRAM

    # Persistent, non-initialized memory.
    GROUP: {
        .persistent? NOLOAD: {
            *(.persistent*)
        }
    } > PRAM

    # Persistent memory.
    GROUP: {
        # TLS, thread local storage
        .tls: {
            *(.tls*)
        }
        .data: {
            *(.data*)
        }
        .sdata?: {
            *(.sdata*)
        }
        .sbss?: {
            *(.sbss*)
        }
        .bss?: {
            *(.bss*)
        }
        .heap? ALIGN(4) SIZE(DEFINED _HEAPSIZE ? _HEAPSIZE : 0): {
            *(.heap*)
        }

        # End of used PRAM. Used for setting DRAM into retention mode.
        MEMORY_PRAM_USED_END = .;
        MEMORY_NVM_PRAM_USED_END = .;
    } > PRAM

    # Force the linker to ensure there is enough space for the stack.
    GROUP: {
        .stack_allocated_space NOLOAD ADDR(_fstack) ALIGN(4) SIZE(_STACKSIZE): {}
    } > NPRAM

    # Non-persistent, non-initialized memory (lost during sleep).
    GROUP: {
        .noinit? NOLOAD: {
            # Variables defined with NO_INIT.
            *(.noinit*)
        }
    } > NPRAM

    # Non-persistent memory (lost during sleep).
    GROUP: {
        .config: {
            *(.master_config*)
            *(.config*)
        }
        .non_persistent?: {
            *(.non_persistent*)
        }
    } > NPRAM

    # Non-persistent memory of the ROM.
    GROUP: {
        .rom_used_npram NOLOAD ADDR(_MEMORY_ROM_NPRAM_USED_START) SIZE(ROM_USED_NPRAM_SIZE): {}
    } > NPRAM

    # NVMINFO: Non-volatile storage for data and configuration.
    # Do not move this group after the nvm group else .nvm* would
    # include .nvm_info sections.
    # The unused memory space is filled by 0xFFFF.
    GROUP: {
        .nvm_info_const TYPE(data): {
            *(.nvm_info_const*)
        }

        .nvm_info TYPE(text): {
            *(.nvm_info*)
        }
    } > NVMINFO

    # NVMENTRY: Entry section for application startup in NVM
    # Do not move this group after the nvm group else .nvm* would
    # include .nvm_mbr sections.
    # The unused memory space is filled by 0xFFFF.

    GROUP ADDR(NVMBL_ENTRY_ADDR): {
        .nvm_bootloader TYPE(text): {
            KEEP(*(.nvm_bootloader*))
        }
    } > NVM

    GROUP ADDR(NVM_ORIGIN): {
        .programHeader TYPE(text): {
            KEEP(*(.programHeader*))
        }
        PAYLOAD_START = .;
        .nvm_entry TYPE(text): {
            *(.nvm_entry*)
        }
    } > NVM

    # NVM: Non-volatile storage for application, data and configuration.
    # Do not move this group before the nvm info group else .nvm* would
    # include .nvm_info sections.
    # The unused memory space is filled by 0xFFFF.
    # NVM: Main Flash firmware
    GROUP NEXT(0x400): {
        # Interrupt vector table.
        MEMORY_IVT_START = .;
        .vectors TYPE(text): {
            *(.vectors*)
        }
        MEMORY_IVT_END = .;

        # Main firmware.
        *(TEXT): {}

        # ROM/NVM constants is going to NVM
        .rom_nvm_const?: {
            *(.rom_nvm_const*)
        }

        # NVM constants (constants not copied into RAM).
        .nvm_const?: {
            *(.nvm_const*)
        }

        .const?: {
            *(.const*)
            *(.rom_const*)
            *(.irom_const*)
        }

        .rodata?: {
            *(.rodata*)
        }

        # JLI table and section initialization data structure (initdat).
        .initdat?: {
            *(.initdat*)
        }
        *(LIT): {} # .rodata

        .ctors?: {
            *(.ctors*)
            *(.dtors*)
            *(.eh_frame*)
        }
        PAYLOAD_END = .;
        PAYLOAD_SIZE = PAYLOAD_END - PAYLOAD_START;
    } > NVM = 0xFFFFFFFF
    GROUP NEXT(0x2000): {
        APP_ENTRY_ADDR = .;
        .empty_app_entry (TEXT) SIZE(DEFINED(APP_IS_EMCORE) ? 4 : 0): {
        } = FILL(0xFFFFFFFF, 4)
    } > NVM
}
