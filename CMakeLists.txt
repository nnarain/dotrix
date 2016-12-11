#
# Dotrix Build
#
# @author Natesh Narain <nnaraindev@gmail.com>
# @date Nov 10 2016
#

cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

project(dotrix)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

# Qt Configuration
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)

get_target_property(QtCore_location Qt5::Core LOCATION)

# version info
include(version-config)
generate_version_info()

# Group UI files
set(UI_FILES
    ui/mainwindow.ui
	ui/network_settings.ui
)
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# Resource file
qt5_add_resources(RESOURCE_FILES )

# GameboyCore config
find_package(GameboyCore)

include_directories(
    include/
    ${GAMEBOYCORE_INCLUDE_DIR}
)

# group headers
set(HEADERS
    include/mainwindow.h
	include/network_settings.h
	include/screen.h
	
	include/emu/qgameboycore.h
	include/emu/core_updater.h
	include/emu/input.h
)

add_executable(${PROJECT_NAME}
    ${UI_HEADERS}
    ${HEADERS}
    src/main.cpp
    src/mainwindow.cpp
	src/network_settings.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
	Qt5::Gui
    ${GAMEBOYCORE_LIBRARIES}
)

add_dependencies(${PROJECT_NAME} version)

# install targets and deps
get_filename_component(QT_BIN ${QtCore_location} DIRECTORY)

install(
    TARGETS dotrix
    RUNTIME DESTINATION bin
)

if(WIN32)
    install(
        FILES
            ${QT_BIN}/Qt5Core.dll
            ${QT_BIN}/Qt5Widgets.dll
            ${QT_BIN}/Qt5Gui.dll
            ${GAMEBOYCORE_BIN_DIR}/gameboycore.dll
        DESTINATION bin
    )
endif(WIN32)

# package

include(get-version-config)
get_version()

set(CPACK_PACKAGE_NAME "Dotrix")
set(CPACK_PACKAGE_VENDOR "nnarain")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Gameboy Emulator UI")
set(CPACK_PACKAGE_VERSION "${VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Dotrix")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

include(CPack)
