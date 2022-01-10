cmake_minimum_required(VERSION 3.14)
project(drumex)

set(CMAKE_CXX_STANDARD 17)

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
        time_signature/TimeSignature.cpp
        time_signature/TimeSignature.h
        Translations.h
        variations/Variations.cpp
        variations/Variations.h)

# ------- Finding SDL Sources and Libraries ---------- #

Message("")
Message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
Message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
Message(STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND})
Message(STATUS "CMAKE_VERSION: " ${CMAKE_VERSION})
Message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
Message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
Message(STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")

FIND_PACKAGE(SDL2 REQUIRED)
Message("")
Message(STATUS "FINDING SDL2")
IF (${SDL2_FOUND})
    Message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message(STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
    Message(STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
ELSE ()
    Message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
    Message(FATAL_ERROR "SDL2 NOT FOUND")
ENDIF ()

FIND_PACKAGE(SDL2_IMAGE)
Message("")
Message(STATUS "FINDING SDL2_IMAGE")
Message(STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
IF (${SDL2_IMAGE_FOUND})
    Message(STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
    Message(STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
ELSE ()
    Message(FATAL_ERROR "SDL2_IMAGE NOT FOUND")
ENDIF ()

FIND_PACKAGE(SDL2TTF)
Message("")
Message(STATUS "FINDING SDL2TTF")
Message(STATUS "SDL2TTF_FOUND: " ${SDL2TTF_FOUND})
IF (${SDL2TTF_FOUND})
    Message(STATUS "SDL2TTF_INCLUDE_DIR: " ${SDL2TTF_INCLUDE_DIR})
    Message(STATUS "SDL2TTF_LIBRARY: " ${SDL2TTF_LIBRARY})
ELSE ()
    Message(FATAL_ERROR "SDL2TTF NOT FOUND")
ENDIF ()
Message("")

FIND_PACKAGE(SDL2Mixer)

Message(STATUS "FINDING SDL2Mixer")
Message(STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND})

Message("")


# ------- End Finding SDL ------ #

# todo: locate jsoncpp paths nicely.
INCLUDE_DIRECTORIES(
        /usr/local/include
        ${SDL2_INCLUDE_DIR}
        ${SDL2TTF_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2Mixer_INCLUDE_DIR}
        /usr/local/Cellar/jsoncpp/1.9.4_1/include/)
target_link_libraries(${PROJECT_NAME}
        pthread
        ${SDL2_LIBRARY}
        ${SDL2TTF_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2Mixer_LIBRARY}
        /usr/local/Cellar/jsoncpp/1.9.4_1/lib/libjsoncpp.dylib)
