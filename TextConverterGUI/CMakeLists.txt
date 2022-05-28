# TextConverter GUI tool

cmake_minimum_required(VERSION 3.20)

#project(TextConverterGUI VERSION 1.0.0 LANGUAGES CXX)
LIST(APPEND CMAKE_PREFIX_PATH "c:/program files (x86)/Qt/lib/cmake/")
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)

add_executable(TextConverterGUI
	main.cpp 
	rasterwindow.cpp
)

# include TextConverter library
target_include_directories(TextConverterGUI PUBLIC ${ToolLibraryIncDir})

# include libraries
target_link_libraries( TextConverterGUI PUBLIC 
	Qt6::Widgets
	Qt6::Network
	Qt6::Core
	Qt6::Gui
    TextConverterLib)

# Resources:
# set(application_resource_files
#     "images/copy.png"
#     "images/cut.png"
#     "images/new.png"
#     "images/open.png"
#     "images/paste.png"
#     "images/save.png"
# )

# qt6_add_resources(TextConverterGUI "application"
#     PREFIX
#         "/"
#     FILES
#         ${application_resource_files}
# )

install(TARGETS TextConverterGUI DESTINATION bin)
install(FILES "c:/Program Files (x86)/Qt/bin/Qt6Cored.dll" DESTINATION bin)
install(FILES "c:/Program Files (x86)/Qt/bin/Qt6Guid.dll" DESTINATION bin)
install(FILES "C:/Program Files (x86)/Qt/plugins/platforms/qwindowsd.dll" DESTINATION bin/platforms)
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")

set(CPACK_NSIS_MODIFY_PATH ON)
include(CPack)