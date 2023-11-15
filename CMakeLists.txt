cmake_minimum_required(VERSION 3.24)
project(graph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(expression REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra")
set(CMAKE_PREFIX_PATH "C:/Qt/6.4.2/mingw_64/lib/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
option(BUILD_SHARED_LIBS "" OFF)


set(Qt6Libs Core Widgets Gui PrintSupport)
find_package(Qt6 COMPONENTS
        Core Widgets Gui PrintSupport
        REQUIRED)

include(FetchContent)
FetchContent_Declare(qcustomplot
        URL https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz)
FetchContent_MakeAvailable(qcustomplot)
add_library(qcustomplot
        "${qcustomplot_SOURCE_DIR}/qcustomplot.h"
        "${qcustomplot_SOURCE_DIR}/qcustomplot.cpp"
        )
target_include_directories(qcustomplot PUBLIC
        "${qcustomplot_SOURCE_DIR}"
        )


foreach(i IN LISTS Qt6Libs)
    target_link_libraries(qcustomplot PUBLIC
            Qt::${i}
            )
endforeach()
set_target_properties(qcustomplot PROPERTIES
        AUTOMOC ON
        )

set(RELEASE_EXECUTABLE "")
if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(RELEASE_EXECUTABLE WIN32)
endif()

add_executable(graph ${RELEASE_EXECUTABLE} main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        )


target_link_libraries(graph PUBLIC
        expression::expression
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::PrintSupport
        qcustomplot
        )

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
