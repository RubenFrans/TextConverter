# TextConverterCommandLineTool
project(TextConverterCommandLineTool)
add_executable(TextConverterCommandLineTool main.cpp)

# set cpp standard
target_compile_features(TextConverterCommandLineTool PRIVATE cxx_std_17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#target_include_directories(TextConverterCommandLineTool PUBLIC ${ToolLibraryIncDir})
target_include_directories(TextConverterCommandLineTool PUBLIC ${ToolLibraryIncDir})
target_link_libraries(TextConverterCommandLineTool PUBLIC TextConverterLib)

install(TARGETS TextConverterCommandLineTool DESTINATION bin)

set(CPACK_PACKAGE_NAME "TextConverterCommandLineTool")
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_MODIFY_PATH ON)



include(CPack)