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

# Utility rule file for nvm_emb_hrs.

# Include any custom commands dependencies for this target.
include projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/compiler_depend.make

# Include the progress variables for this target.
include projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/progress.make

projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs: projects/nvm_emb_hrs/nvm_emb_hrs_di05.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating nvm_emb_hrs"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && "C:/Program Files/CMake/bin/cmake.exe" -E rename nvm_emb_hrs_di05.ihex nvm_emb_hrs.ihex
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && "C:/Program Files/CMake/bin/cmake.exe" -E rename nvm_emb_hrs_di05.asm nvm_emb_hrs.asm
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && "C:/Program Files/CMake/bin/cmake.exe" -E rename nvm_emb_hrs_di05.sym nvm_emb_hrs.sym
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && "C:/Program Files/CMake/bin/cmake.exe" -E rename nvm_emb_hrs_di05.elf nvm_emb_hrs.elf
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && "C:/Program Files/CMake/bin/cmake.exe" -E rename nvm_emb_hrs_di05.map nvm_emb_hrs.map
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && "C:/Program Files/CMake/bin/cmake.exe" -E rename nvm_emb_hrs_di05_valid_hdr.elf nvm_emb_hrs_valid_hdr.elf

projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/codegen:
.PHONY : projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/codegen

nvm_emb_hrs: projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs
nvm_emb_hrs: projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/build.make
.PHONY : nvm_emb_hrs

# Rule to build all files generated by this target.
projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/build: nvm_emb_hrs
.PHONY : projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/build

projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/clean:
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" && $(CMAKE_COMMAND) -P CMakeFiles/nvm_emb_hrs.dir/cmake_clean.cmake
.PHONY : projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/clean

projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/nvm_emb_hrs" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : projects/nvm_emb_hrs/CMakeFiles/nvm_emb_hrs.dir/depend

