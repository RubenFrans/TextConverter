# TextConverter GUI tool

cmake_minimum_required(VERSION 3.20)
project(TextConverterGraphics)

LIST(APPEND CMAKE_PREFIX_PATH "c:/program files (x86)/Qt/lib/cmake/")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Gui REQUIRED)

	#rasterwindow.cpp
add_executable(TextConverterGUI
	TextConverterForm.cpp
	TextConverterForm.h
	TextConverterForm.ui
	main.cpp 
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


install(TARGETS TextConverterGUI DESTINATION bin)
install(FILES "c:/Program Files (x86)/Qt/bin/Qt6Cored.dll" DESTINATION bin)
install(FILES "c:/Program Files (x86)/Qt/bin/Qt6Guid.dll" DESTINATION bin)
install(FILES "c:/Program Files (x86)/Qt/bin/Qt6Widgetsd.dll" DESTINATION bin)
install(FILES "C:/Program Files (x86)/Qt/plugins/platforms/qwindowsd.dll" DESTINATION bin/platforms)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
 
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_MODIFY_PATH ON)

include(CPack)