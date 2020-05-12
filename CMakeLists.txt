cmake_minimum_required(VERSION 3.14)
project(drumex)

set(CMAKE_CXX_STANDARD 17)

add_executable(drumex main.cpp
        display/Display.cpp
        display/Display.h
        display/DisplayConfigurations.h
        Enums.h
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
        variations/Variations.h
        location/Location.cpp
        location/Location.h)

target_link_libraries(drumex pthread)
target_link_libraries(drumex SDL2)
target_link_libraries(drumex SDL2_mixer)
target_link_libraries(drumex SDL2_image)
target_link_libraries(drumex SDL2_ttf)
target_link_libraries(drumex jsoncpp)