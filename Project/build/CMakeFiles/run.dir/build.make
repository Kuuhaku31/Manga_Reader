# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "E:\Microsoft VS Code\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "E:\Microsoft VS Code\cmake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\Project\VS Code\Manga_Reader\Project"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\Project\VS Code\Manga_Reader\Project\build"

# Include any dependencies generated for this target.
include CMakeFiles/run.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/run.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/run.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/run.dir/flags.make

CMakeFiles/run.dir/src/IO.cpp.obj: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/src/IO.cpp.obj: CMakeFiles/run.dir/includes_CXX.rsp
CMakeFiles/run.dir/src/IO.cpp.obj: D:/Project/VS\ Code/Manga_Reader/Project/src/IO.cpp
CMakeFiles/run.dir/src/IO.cpp.obj: CMakeFiles/run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/run.dir/src/IO.cpp.obj"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run.dir/src/IO.cpp.obj -MF CMakeFiles\run.dir\src\IO.cpp.obj.d -o CMakeFiles\run.dir\src\IO.cpp.obj -c "D:\Project\VS Code\Manga_Reader\Project\src\IO.cpp"

CMakeFiles/run.dir/src/IO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run.dir/src/IO.cpp.i"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\VS Code\Manga_Reader\Project\src\IO.cpp" > CMakeFiles\run.dir\src\IO.cpp.i

CMakeFiles/run.dir/src/IO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run.dir/src/IO.cpp.s"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\VS Code\Manga_Reader\Project\src\IO.cpp" -o CMakeFiles\run.dir\src\IO.cpp.s

CMakeFiles/run.dir/src/Input.cpp.obj: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/src/Input.cpp.obj: CMakeFiles/run.dir/includes_CXX.rsp
CMakeFiles/run.dir/src/Input.cpp.obj: D:/Project/VS\ Code/Manga_Reader/Project/src/Input.cpp
CMakeFiles/run.dir/src/Input.cpp.obj: CMakeFiles/run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/run.dir/src/Input.cpp.obj"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run.dir/src/Input.cpp.obj -MF CMakeFiles\run.dir\src\Input.cpp.obj.d -o CMakeFiles\run.dir\src\Input.cpp.obj -c "D:\Project\VS Code\Manga_Reader\Project\src\Input.cpp"

CMakeFiles/run.dir/src/Input.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run.dir/src/Input.cpp.i"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\VS Code\Manga_Reader\Project\src\Input.cpp" > CMakeFiles\run.dir\src\Input.cpp.i

CMakeFiles/run.dir/src/Input.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run.dir/src/Input.cpp.s"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\VS Code\Manga_Reader\Project\src\Input.cpp" -o CMakeFiles\run.dir\src\Input.cpp.s

CMakeFiles/run.dir/src/MangaBook.cpp.obj: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/src/MangaBook.cpp.obj: CMakeFiles/run.dir/includes_CXX.rsp
CMakeFiles/run.dir/src/MangaBook.cpp.obj: D:/Project/VS\ Code/Manga_Reader/Project/src/MangaBook.cpp
CMakeFiles/run.dir/src/MangaBook.cpp.obj: CMakeFiles/run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/run.dir/src/MangaBook.cpp.obj"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run.dir/src/MangaBook.cpp.obj -MF CMakeFiles\run.dir\src\MangaBook.cpp.obj.d -o CMakeFiles\run.dir\src\MangaBook.cpp.obj -c "D:\Project\VS Code\Manga_Reader\Project\src\MangaBook.cpp"

CMakeFiles/run.dir/src/MangaBook.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run.dir/src/MangaBook.cpp.i"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\VS Code\Manga_Reader\Project\src\MangaBook.cpp" > CMakeFiles\run.dir\src\MangaBook.cpp.i

CMakeFiles/run.dir/src/MangaBook.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run.dir/src/MangaBook.cpp.s"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\VS Code\Manga_Reader\Project\src\MangaBook.cpp" -o CMakeFiles\run.dir\src\MangaBook.cpp.s

CMakeFiles/run.dir/src/Reader.cpp.obj: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/src/Reader.cpp.obj: CMakeFiles/run.dir/includes_CXX.rsp
CMakeFiles/run.dir/src/Reader.cpp.obj: D:/Project/VS\ Code/Manga_Reader/Project/src/Reader.cpp
CMakeFiles/run.dir/src/Reader.cpp.obj: CMakeFiles/run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/run.dir/src/Reader.cpp.obj"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run.dir/src/Reader.cpp.obj -MF CMakeFiles\run.dir\src\Reader.cpp.obj.d -o CMakeFiles\run.dir\src\Reader.cpp.obj -c "D:\Project\VS Code\Manga_Reader\Project\src\Reader.cpp"

CMakeFiles/run.dir/src/Reader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run.dir/src/Reader.cpp.i"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\VS Code\Manga_Reader\Project\src\Reader.cpp" > CMakeFiles\run.dir\src\Reader.cpp.i

CMakeFiles/run.dir/src/Reader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run.dir/src/Reader.cpp.s"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\VS Code\Manga_Reader\Project\src\Reader.cpp" -o CMakeFiles\run.dir\src\Reader.cpp.s

CMakeFiles/run.dir/src/Volume.cpp.obj: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/src/Volume.cpp.obj: CMakeFiles/run.dir/includes_CXX.rsp
CMakeFiles/run.dir/src/Volume.cpp.obj: D:/Project/VS\ Code/Manga_Reader/Project/src/Volume.cpp
CMakeFiles/run.dir/src/Volume.cpp.obj: CMakeFiles/run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/run.dir/src/Volume.cpp.obj"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run.dir/src/Volume.cpp.obj -MF CMakeFiles\run.dir\src\Volume.cpp.obj.d -o CMakeFiles\run.dir\src\Volume.cpp.obj -c "D:\Project\VS Code\Manga_Reader\Project\src\Volume.cpp"

CMakeFiles/run.dir/src/Volume.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run.dir/src/Volume.cpp.i"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\VS Code\Manga_Reader\Project\src\Volume.cpp" > CMakeFiles\run.dir\src\Volume.cpp.i

CMakeFiles/run.dir/src/Volume.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run.dir/src/Volume.cpp.s"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\VS Code\Manga_Reader\Project\src\Volume.cpp" -o CMakeFiles\run.dir\src\Volume.cpp.s

CMakeFiles/run.dir/src/main.cpp.obj: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/src/main.cpp.obj: CMakeFiles/run.dir/includes_CXX.rsp
CMakeFiles/run.dir/src/main.cpp.obj: D:/Project/VS\ Code/Manga_Reader/Project/src/main.cpp
CMakeFiles/run.dir/src/main.cpp.obj: CMakeFiles/run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/run.dir/src/main.cpp.obj"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/run.dir/src/main.cpp.obj -MF CMakeFiles\run.dir\src\main.cpp.obj.d -o CMakeFiles\run.dir\src\main.cpp.obj -c "D:\Project\VS Code\Manga_Reader\Project\src\main.cpp"

CMakeFiles/run.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/run.dir/src/main.cpp.i"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\Project\VS Code\Manga_Reader\Project\src\main.cpp" > CMakeFiles\run.dir\src\main.cpp.i

CMakeFiles/run.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/run.dir/src/main.cpp.s"
	"E:\Microsoft VS Code\TDM-GCC-64\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\Project\VS Code\Manga_Reader\Project\src\main.cpp" -o CMakeFiles\run.dir\src\main.cpp.s

# Object files for target run
run_OBJECTS = \
"CMakeFiles/run.dir/src/IO.cpp.obj" \
"CMakeFiles/run.dir/src/Input.cpp.obj" \
"CMakeFiles/run.dir/src/MangaBook.cpp.obj" \
"CMakeFiles/run.dir/src/Reader.cpp.obj" \
"CMakeFiles/run.dir/src/Volume.cpp.obj" \
"CMakeFiles/run.dir/src/main.cpp.obj"

# External object files for target run
run_EXTERNAL_OBJECTS =

D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/src/IO.cpp.obj
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/src/Input.cpp.obj
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/src/MangaBook.cpp.obj
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/src/Reader.cpp.obj
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/src/Volume.cpp.obj
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/src/main.cpp.obj
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/build.make
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: D:/Project/VS\ Code/Manga_Reader/Project/easyx/libeasyx.a
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/linkLibs.rsp
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/objects1.rsp
D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe: CMakeFiles/run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable \"D:\Project\VS Code\Manga_Reader\Project\bin\run.exe\""
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\run.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/run.dir/build: D:/Project/VS\ Code/Manga_Reader/Project/bin/run.exe
.PHONY : CMakeFiles/run.dir/build

CMakeFiles/run.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\run.dir\cmake_clean.cmake
.PHONY : CMakeFiles/run.dir/clean

CMakeFiles/run.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\Project\VS Code\Manga_Reader\Project" "D:\Project\VS Code\Manga_Reader\Project" "D:\Project\VS Code\Manga_Reader\Project\build" "D:\Project\VS Code\Manga_Reader\Project\build" "D:\Project\VS Code\Manga_Reader\Project\build\CMakeFiles\run.dir\DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/run.dir/depend

