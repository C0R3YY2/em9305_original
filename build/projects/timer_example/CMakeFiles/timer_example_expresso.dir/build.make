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

# Utility rule file for timer_example_expresso.

# Include any custom commands dependencies for this target.
include projects/timer_example/CMakeFiles/timer_example_expresso.dir/compiler_depend.make

# Include the progress variables for this target.
include projects/timer_example/CMakeFiles/timer_example_expresso.dir/progress.make

projects/timer_example/CMakeFiles/timer_example_expresso: projects/timer_example/timer_example_expresso_di05.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir="C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating timer_example_expresso"
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" -E rename timer_example_expresso_di05.ihex timer_example_expresso.ihex
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" -E rename timer_example_expresso_di05.asm timer_example_expresso.asm
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" -E rename timer_example_expresso_di05.sym timer_example_expresso.sym
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" -E rename timer_example_expresso_di05.elf timer_example_expresso.elf
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" -E rename timer_example_expresso_di05.map timer_example_expresso.map
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && "C:/Program Files/CMake/bin/cmake.exe" -E rename timer_example_expresso_di05_valid_hdr.elf timer_example_expresso_valid_hdr.elf

projects/timer_example/CMakeFiles/timer_example_expresso.dir/codegen:
.PHONY : projects/timer_example/CMakeFiles/timer_example_expresso.dir/codegen

timer_example_expresso: projects/timer_example/CMakeFiles/timer_example_expresso
timer_example_expresso: projects/timer_example/CMakeFiles/timer_example_expresso.dir/build.make
.PHONY : timer_example_expresso

# Rule to build all files generated by this target.
projects/timer_example/CMakeFiles/timer_example_expresso.dir/build: timer_example_expresso
.PHONY : projects/timer_example/CMakeFiles/timer_example_expresso.dir/build

projects/timer_example/CMakeFiles/timer_example_expresso.dir/clean:
	cd "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" && $(CMAKE_COMMAND) -P CMakeFiles/timer_example_expresso.dir/cmake_clean.cmake
.PHONY : projects/timer_example/CMakeFiles/timer_example_expresso.dir/clean

projects/timer_example/CMakeFiles/timer_example_expresso.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/projects/timer_example" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example" "C:/Program Files/EM_Microelectronic/EMB-SDK/EM9305_EM_BLEU_SDK_v4.2/build/projects/timer_example/CMakeFiles/timer_example_expresso.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : projects/timer_example/CMakeFiles/timer_example_expresso.dir/depend

