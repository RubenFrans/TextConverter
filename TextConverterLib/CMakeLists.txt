# TextConverterLib project build file in subdirectory TextConverterLib 
# Contains library to be used by CommandLine and GUI Tool

add_library(TextConverterLib TextConverter.cpp TextConverter.h)
set(ToolLibraryIncDir ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE)