cmake_minimum_required(VERSION 3.14)
project(drumex)

set(CMAKE_CXX_STANDARD 20)

add_executable(drumex main.cpp
        display/Display.cpp
        display/Display.h
        display/DisplayConfigurations.h
        Enums.h
        location/Location.cpp
        location/Location.h
        logging/Logger.cpp
        logging/Logger.h
        metronome/Metronome.cpp
        metronome/Metronome.h
        notation/Fraction.cpp
        notation/Fraction.h
        notation/Notation.cpp
        notation/Notation.h
        notation/NotationDisplay.cpp
        notation/NotationDisplay.h
        notation/NotationDisplayUtils.cpp
        notation/NotationDisplayUtils.h
        notation/NotationUtils.cpp
        notation/NotationUtils.h
        notation_container/BarContainer.cpp
        notation_container/BarContainer.h
        notation_container/RhythmContainer.cpp
        notation_container/RhythmContainer.h
        notation_container/VoiceContainer.cpp
        notation_container/VoiceContainer.h
        reader/Exercise.cpp
        reader/Exercise.h
        reader/Part.cpp
        reader/Part.h
        resources_menu/ResourcesMenu.cpp
        resources_menu/ResourcesMenu.h
        time_signature/TimeSignature.cpp
        time_signature/TimeSignature.h
        Translations.h
        variations/Variations.cpp
        variations/Variations.h)

add_subdirectory(imgui)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    SET(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

# todo: locate jsoncpp paths nicely.
target_include_directories(drumex PUBLIC
        imgui
        ${SDL_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        /usr/local/Cellar/jsoncpp/1.9.5/include)
target_link_libraries(drumex
        imgui
        pthread
        ${SDL_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${EXTRA_LIBS}
        /usr/local/Cellar/jsoncpp/1.9.5/lib/libjsoncpp.dylib)
