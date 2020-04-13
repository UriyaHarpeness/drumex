cmake_minimum_required(VERSION 3.14)
project(drumex)

set(CMAKE_CXX_STANDARD 17)

add_executable(drumex main.cpp
        Enums.h
        metronome/Metronome.cpp
        metronome/Metronome.h
        variations/Variations.h
        variations/Variations.cpp
        display/Display.cpp
        display/Display.h
        notation/Notation.cpp
        notation/Notation.h
        notation/NotationUtils.cpp
        notation/NotationUtils.h
        notation/NotationDisplay.cpp
        notation/NotationDisplay.h
        notation/Fraction.cpp
        notation/Fraction.h
        Translations.h
        reader/Part.cpp
        reader/Part.h
        reader/Exercise.cpp
        reader/Exercise.h
        location/Location.cpp
        location/Location.h
        time_signature/TimeSignature.cpp
        time_signature/TimeSignature.h
        notation_container/VoiceContainer.cpp
        notation_container/VoiceContainer.h
        notation_container/BarContainer.cpp
        notation_container/BarContainer.h
        notation_container/RhythmContainer.cpp
        notation_container/RhythmContainer.h)

target_link_libraries(drumex pthread)
target_link_libraries(drumex SDL2)
target_link_libraries(drumex SDL2_mixer)
target_link_libraries(drumex SDL2_image)
target_link_libraries(drumex SDL2_ttf)
target_link_libraries(drumex jsoncpp)