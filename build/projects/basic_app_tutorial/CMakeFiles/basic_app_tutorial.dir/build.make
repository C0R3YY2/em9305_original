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

# Utility rule file for basic_app_tutorial.

# Include any custom commands dependencies for this target.
include projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/compiler_depend.make

# Include the progress variables for this target.
include projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/progress.make

projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial: projects/basic_app_tutorial/basic_app_tutorial_di05.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating basic_app_tutorial"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && "C:/Program Files/CMake/bin/cmake.exe" -E rename basic_app_tutorial_di05.ihex basic_app_tutorial.ihex
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && "C:/Program Files/CMake/bin/cmake.exe" -E rename basic_app_tutorial_di05.asm basic_app_tutorial.asm
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && "C:/Program Files/CMake/bin/cmake.exe" -E rename basic_app_tutorial_di05.sym basic_app_tutorial.sym
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && "C:/Program Files/CMake/bin/cmake.exe" -E rename basic_app_tutorial_di05.elf basic_app_tutorial.elf
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && "C:/Program Files/CMake/bin/cmake.exe" -E rename basic_app_tutorial_di05.map basic_app_tutorial.map
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && "C:/Program Files/CMake/bin/cmake.exe" -E rename basic_app_tutorial_di05_valid_hdr.elf basic_app_tutorial_valid_hdr.elf

projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/codegen:
.PHONY : projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/codegen

basic_app_tutorial: projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial
basic_app_tutorial: projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/build.make
.PHONY : basic_app_tutorial

# Rule to build all files generated by this target.
projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/build: basic_app_tutorial
.PHONY : projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/build

projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/clean:
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" && $(CMAKE_COMMAND) -P CMakeFiles/basic_app_tutorial.dir/cmake_clean.cmake
.PHONY : projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/clean

projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/basic_app_tutorial" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : projects/basic_app_tutorial/CMakeFiles/basic_app_tutorial.dir/depend

