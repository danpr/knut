# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dan/Projekty/knut/knutsetting4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dan/Projekty/knut/knutsetting4/build

# Utility rule file for actions for org.kde.kcontrol.knutsetting.

CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting: org.kde.kcontrol.knutsetting.policy

org.kde.kcontrol.knutsetting.policy: ../knutsetting.actions
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dan/Projekty/knut/knutsetting4/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating org.kde.kcontrol.knutsetting.policy"
	cd /home/dan/Projekty/knut/knutsetting4 && /usr/lib/kde4/libexec/kauth-policy-gen /home/dan/Projekty/knut/knutsetting4/knutsetting.actions > /home/dan/Projekty/knut/knutsetting4/build/org.kde.kcontrol.knutsetting.policy

actions\ for\ org.kde.kcontrol.knutsetting: CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting
actions\ for\ org.kde.kcontrol.knutsetting: org.kde.kcontrol.knutsetting.policy
actions\ for\ org.kde.kcontrol.knutsetting: CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/build.make
.PHONY : actions\ for\ org.kde.kcontrol.knutsetting

# Rule to build all files generated by this target.
CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/build: actions\ for\ org.kde.kcontrol.knutsetting
.PHONY : CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/build

CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/clean:
	$(CMAKE_COMMAND) -P "CMakeFiles/actions for org.kde.kcontrol.knutsetting.dir/cmake_clean.cmake"
.PHONY : CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/clean

CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/depend:
	cd /home/dan/Projekty/knut/knutsetting4/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dan/Projekty/knut/knutsetting4 /home/dan/Projekty/knut/knutsetting4 /home/dan/Projekty/knut/knutsetting4/build /home/dan/Projekty/knut/knutsetting4/build "/home/dan/Projekty/knut/knutsetting4/build/CMakeFiles/actions for org.kde.kcontrol.knutsetting.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/actions\ for\ org.kde.kcontrol.knutsetting.dir/depend

