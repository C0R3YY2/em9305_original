# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build"

# Include any dependencies generated for this target.
include projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/compiler_depend.make

# Include the progress variables for this target.
include projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/progress.make

# Include the compile flags for this target's objects.
include projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/flags.make

projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/codegen:
.PHONY : projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/codegen

projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj: projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/flags.make
projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/timer_example/nvm_main_oneshot.c
projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/common/9305/includes/jli.h
projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj: projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && C:/ARC/MetaWare/arc/bin/ccac.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj -MF CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj.d -o CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj -c "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/timer_example/nvm_main_oneshot.c"

projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.i"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && C:/ARC/MetaWare/arc/bin/ccac.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/timer_example/nvm_main_oneshot.c" > CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.i

projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.s"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && C:/ARC/MetaWare/arc/bin/ccac.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/timer_example/nvm_main_oneshot.c" -o CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.s

# Object files for target timer_example_oneshot_di03
timer_example_oneshot_di03_OBJECTS = \
"CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj"

# External object files for target timer_example_oneshot_di03
timer_example_oneshot_di03_EXTERNAL_OBJECTS =

projects/timer_example/timer_example_oneshot_di03.elf: projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/nvm_main_oneshot.c.obj
projects/timer_example/timer_example_oneshot_di03.elf: projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/build.make
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/third_party/Metaware/lib_Metaware.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_core/lib_em_core.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/firmware_header/lib_header_access_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_hw_api/lib_em_hw_api_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_system/lib_em_system_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm_entry/lib_nvm_entry_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/pml/lib_pml_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/radio/lib_radio.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/aoad/lib_aoad.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/prot_timer/lib_prot_timer.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/sleep_timer/lib_sleep_timer.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/rc_calib/lib_rc_calib.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/sleep_manager/lib_sleep_manager.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/transport/lib_transport.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/memory_manager/lib_memory_manager.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/dma/lib_dma.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/gpio/lib_gpio_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/spi_slave/lib_spi_slave_dma.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/uart/lib_uart_dma.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm_info/lib_nvm_info.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/lib_hw_security_di03.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm/lib_nvm_fix.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CBC_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CCM_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CMAC_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CTR_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_ECB_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_GCM_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/ECC_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/PRNG_ARC.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/unitimer/lib_unitimer.a
projects/timer_example/timer_example_oneshot_di03.elf: C:/Program\ Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/rom/bin/v1.0/rom.sym
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable timer_example_oneshot_di03.elf"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && C:/ARC/MetaWare/arc/bin/ccac.exe  -tcf=em9305 -arcv2em -HL -Hlpc_width=16 -Hpc_width=24 -Xcode_density -Xdiv_rem=radix4 -Xswap -Xbitscan -Xshift_assist -Xbarrel_shifter -Xfpus_div -Xfpus_mpy_slow -Xfpus_div_slow -Xtimer0 -Xstack_check -Hccm   -Wall -Wconversion -Hhostlib= -Hnocrt -Hon=Long_enums -Hnocopyr -Hnosdata -nostdsysteminc -Hccm -mllvm -align-labels=false -Hnoivt -ffunction-sections -fdata-sections -Wl,-Bdefine:STACKSIZE=4096 -Hheap=0 -Wno-cast-align -Mb -Os -g  -tcf=em9305 -arcv2em -HL -Hlpc_width=16 -Hpc_width=24 -Xcode_density -Xdiv_rem=radix4 -Xswap -Xbitscan -Xshift_assist -Xbarrel_shifter -Xfpus_div -Xfpus_mpy_slow -Xfpus_div_slow -Xtimer0 -Xstack_check -Hccm   -Wall -Wconversion -Hhostlib= -Hnocrt -Hon=Long_enums -Hnocopyr -Hnosdata -nostdsysteminc -Hccm -mllvm -align-labels=false -Hnoivt -ffunction-sections -fdata-sections -Wl,-Bdefine:STACKSIZE=4096 -Hheap=0 -Wno-cast-align -Mb -Os  -Wl,-q -Wl,-Bgrouplib -Wl,-Xnocompress -zallow_memory_overlap -Hentry=NVM_Entry -Bno_all_archive -Hpurge -Wl,-A"C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/common/9305/NVM/linker.cmd" -Bsymout="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/timer_example_oneshot_di03.sym" -m -Coutput="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/timer_example_oneshot_di03.map" -Bsymin_weak="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/rom/bin/v1.0/rom.sym" -Bsymin_exclude="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/common/9305/NVM/symlist_exclude.txt" $(timer_example_oneshot_di03_OBJECTS) $(timer_example_oneshot_di03_EXTERNAL_OBJECTS) -o timer_example_oneshot_di03.elf  "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/third_party/Metaware/lib_Metaware.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_core/lib_em_core.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/firmware_header/lib_header_access_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_hw_api/lib_em_hw_api_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/em_system/lib_em_system_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm_entry/lib_nvm_entry_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/pml/lib_pml_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/radio/lib_radio.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/aoad/lib_aoad.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/prot_timer/lib_prot_timer.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/sleep_timer/lib_sleep_timer.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/rc_calib/lib_rc_calib.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/sleep_manager/lib_sleep_manager.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/transport/lib_transport.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/memory_manager/lib_memory_manager.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/dma/lib_dma.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/gpio/lib_gpio_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/spi_slave/lib_spi_slave_dma.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/uart/lib_uart_dma.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm_info/lib_nvm_info.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/lib_hw_security_di03.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/nvm/lib_nvm_fix.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CBC_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CCM_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CMAC_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_CTR_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_ECB_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/AES_GCM_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/ECC_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/security/PRNG_ARC.a" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/libs/unitimer/lib_unitimer.a"
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Generating timer_example_oneshot_di03.ihex"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/cmake/bin/headerstuffer.exe" timer_example_oneshot_di03.elf && elf2hex -QIo timer_example_oneshot_di03.ihex timer_example_oneshot_di03_valid_hdr.elf
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/cmake/bin/head.exe" -n -1 "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/emcore/bin/nvm_bootloader/nvm_bootloader_base_di03.ihex" > timer_example_oneshot_di03_temp.ihex
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/cmake/bin/tail.exe" -n +2 timer_example_oneshot_di03.ihex >> timer_example_oneshot_di03_temp.ihex
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Adding nvm_bootloader_base_di03.ihex to timer_example_oneshot_di03.ihex"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/cmake/bin/mv.exe" timer_example_oneshot_di03_temp.ihex timer_example_oneshot_di03.ihex
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Generating timer_example_oneshot_di03.asm"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" "-DIN=C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/timer_example_oneshot_di03.elf" "-DOUT=C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/timer_example_oneshot_di03.asm" -P "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/cmake/scripts/elfdump.cmake"
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold "Memory usage for timer_example_oneshot_di03.elf"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/cmake/bin/elfstat.exe" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/timer_example_oneshot_di03.elf" --brief

# Rule to build all files generated by this target.
projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/build: projects/timer_example/timer_example_oneshot_di03.elf
.PHONY : projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/build

projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/clean:
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && $(CMAKE_COMMAND) -P CMakeFiles/timer_example_oneshot_di03.dir/cmake_clean.cmake
.PHONY : projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/clean

projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/timer_example" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : projects/timer_example/CMakeFiles/timer_example_oneshot_di03.dir/depend

