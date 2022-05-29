# Root directory of TextConverter project
cmake_minimum_required(VERSION 3.20)
project(TextConverter)

# subproject TextLibrary
add_subdirectory(TextConverterLib)

# subproject TextCommandLineTool
add_subdirectory(TextConverterCommandLineTool)

# subproject TextConverterGUITool
add_subdirectory(TextConverterGUI)

# subproject (Unit tests)
add_subdirectory(UnitTests)
add_dependencies(TextConverterCommandLineTool UnitTestsRun)

include(InstallRequiredSystemLibraries)
include(CPack)