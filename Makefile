# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/e/CD-EL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/CD-EL

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"CheckMalloc\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/e/CD-EL/CMakeFiles /mnt/e/CD-EL//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/e/CD-EL/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named install-CheckMalloc

# Build rule for target.
install-CheckMalloc: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 install-CheckMalloc
.PHONY : install-CheckMalloc

# fast build rule for target.
install-CheckMalloc/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/install-CheckMalloc.dir/build.make CMakeFiles/install-CheckMalloc.dir/build
.PHONY : install-CheckMalloc/fast

#=============================================================================
# Target rules for targets named CheckMalloc

# Build rule for target.
CheckMalloc: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 CheckMalloc
.PHONY : CheckMalloc

# fast build rule for target.
CheckMalloc/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/CheckMalloc.dir/build.make CMakeFiles/CheckMalloc.dir/build
.PHONY : CheckMalloc/fast

#=============================================================================
# Target rules for targets named install-CheckMalloc-stripped

# Build rule for target.
install-CheckMalloc-stripped: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 install-CheckMalloc-stripped
.PHONY : install-CheckMalloc-stripped

# fast build rule for target.
install-CheckMalloc-stripped/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/install-CheckMalloc-stripped.dir/build.make CMakeFiles/install-CheckMalloc-stripped.dir/build
.PHONY : install-CheckMalloc-stripped/fast

#=============================================================================
# Target rules for targets named acc_gen

# Build rule for target.
acc_gen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 acc_gen
.PHONY : acc_gen

# fast build rule for target.
acc_gen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/acc_gen.dir/build.make CMakeFiles/acc_gen.dir/build
.PHONY : acc_gen/fast

#=============================================================================
# Target rules for targets named omp_gen

# Build rule for target.
omp_gen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 omp_gen
.PHONY : omp_gen

# fast build rule for target.
omp_gen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/omp_gen.dir/build.make CMakeFiles/omp_gen.dir/build
.PHONY : omp_gen/fast

#=============================================================================
# Target rules for targets named intrinsics_gen

# Build rule for target.
intrinsics_gen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 intrinsics_gen
.PHONY : intrinsics_gen

# fast build rule for target.
intrinsics_gen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/intrinsics_gen.dir/build.make CMakeFiles/intrinsics_gen.dir/build
.PHONY : intrinsics_gen/fast

CheckMalloc.o: CheckMalloc.cc.o

.PHONY : CheckMalloc.o

# target to build an object file
CheckMalloc.cc.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/CheckMalloc.dir/build.make CMakeFiles/CheckMalloc.dir/CheckMalloc.cc.o
.PHONY : CheckMalloc.cc.o

CheckMalloc.i: CheckMalloc.cc.i

.PHONY : CheckMalloc.i

# target to preprocess a source file
CheckMalloc.cc.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/CheckMalloc.dir/build.make CMakeFiles/CheckMalloc.dir/CheckMalloc.cc.i
.PHONY : CheckMalloc.cc.i

CheckMalloc.s: CheckMalloc.cc.s

.PHONY : CheckMalloc.s

# target to generate assembly for a file
CheckMalloc.cc.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/CheckMalloc.dir/build.make CMakeFiles/CheckMalloc.dir/CheckMalloc.cc.s
.PHONY : CheckMalloc.cc.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... acc_gen"
	@echo "... install-CheckMalloc"
	@echo "... install-CheckMalloc-stripped"
	@echo "... intrinsics_gen"
	@echo "... omp_gen"
	@echo "... CheckMalloc"
	@echo "... CheckMalloc.o"
	@echo "... CheckMalloc.i"
	@echo "... CheckMalloc.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

