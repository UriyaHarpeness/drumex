cmake_minimum_required(VERSION 3.14)
project(drumex)

set(CMAKE_CXX_STANDARD 17)

add_executable(drumex main.cpp
        Enums.h
        metronome/Metronome.cpp
        metronome/Metronome.h
        mixer/Mixer.cpp
        mixer/Mixer.h mixer/Sound.h
        reader/Reader.cpp
        reader/Reader.h
        exercise/Exercise.cpp
        exercise/Exercise.h
        ext.h
        variations/Variations.h
        variations/Variations.cpp display/Display.cpp display/Display.h notation/Notation.cpp notation/Notation.h notation/Fraction.cpp notation/Fraction.h reader/Part.cpp reader/Part.h)

target_link_libraries(drumex pthread)
target_link_libraries(drumex SDL2)
target_link_libraries(drumex SDL2_mixer)
target_link_libraries(drumex SDL2_image)
target_link_libraries(drumex SDL2_ttf)
target_link_libraries(drumex jsoncpp)