cmake_minimum_required(VERSION 3.16)
project(InsightFinderProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Qt ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# --- curl for PersonJsonReader (macOS: installed via Homebrew) ---
# This is MUCH simpler on mac than Windows; no vcpkg/toolchain stuff.
find_package(CURL REQUIRED)

# --- Sources in the Qt subproject ---
set(UI_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.ui
    mainwindow.h
)

# --- Re-use your existing core logic from the parent folder ---
set(CORE_SOURCES
    ../src/AppState.cpp
    ../src/InsightGenerator.cpp
    ../src/InsightStore.cpp
    ../src/Person.cpp
    ../src/PersonBuilder.cpp
    ../src/PersonCsvReader.cpp
    ../src/PersonJsonReader.cpp
    ../src/PersonEnums.cpp
    ../src/PersonRepository.cpp
)

set(CORE_HEADERS
    ../src/AppState.h
    ../src/InsightGenerator.h
    ../src/InsightStore.h
    ../src/Person.h
    ../src/PersonBuilder.h
    ../src/PersonCsvReader.h
    ../src/PersonJsonReader.h
    ../src/PersonEnums.h
    ../src/PersonRepository.h
    ../src/Insight.h
)

qt_add_executable(InsightFinderProject
    ${UI_SOURCES}
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

target_include_directories(InsightFinderProject
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_link_libraries(InsightFinderProject
    PRIVATE
        Qt6::Widgets
        CURL::libcurl
)
