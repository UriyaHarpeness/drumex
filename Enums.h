#pragma once

enum Instrument {
    Unbound,
    SnareInst,
    BassInst,
    HiHatInst,
};

enum Playing {
    Stop,
    Regular,
    Flam,
    Ghost,
    Accent,
};

enum Sticking {
    UnboundStick,
    Right,
    Left,
};

enum MusicSymbolValues {
    SymZero = '0',
    SymOne = '1',
    SymTwo = '2',
    SymThree = '3',
    SymFour = '4',
    SymFive = '5',
    SymSix = '6',
    SymSeven = '7',
    SymEight = '8',
    SymNine = '9',

    SymSmallZero = 0xBC,
    SymSmallOne = 0xC1,
    SymSmallTwo = 0xAA,
    SymSmallThree = 0xA3,
    SymSmallFour = 0xA2,
    SymSmallFive = 0xB0,
    SymSmallSix = 0xA4,
    SymSmallSeven = 0xA6,
    SymSmallEight = 0xA5,
    SymSmallNine = 0xBB,

    SymDoubleWholeNote = 'W',
    SymWholeNote = 'w',

    SymDownHalfNote = 'H',
    SymDownQuarterNote = 'Q',
    SymDownEightNote = 'E',
    SymDownSixteenthNote = 'X',
    SymDownFlag = 'J',

    SymUpHalfNote = 'h',
    SymUpEightNote = 'e',
    SymUpQuarterNote = 'q',
    SymUpSixteenthNote = 'x',
    SymUpFlag = 'j',

    SymDoubleWholeStop = 0xE3,
    SymWholeStop = 0xB7,
    SymHalfStop = 0xB7,
    SymQuarterStop = 0xCE,
    SymEightStop = 0xC5,
    SymSixteenthStop = 0xA8,
    SymThirtySecondStop = 0xF4,
    SymSixtyFourthStop = 0xE5,

    SymStaff = '=',
    SymClef = 0xD6,
    SymDot = 'k',
    SymAccent = '>',
    SymLedger = '_',
    SymTempo = 0xF2,
    SymBarLine = 0x5C,
};

enum MusicSymbol {
    DoubleWholeNote = 'W',
    WholeNote = 'w',
    HalfNote = 'H',
    QuarterNote = 'Q',
    EightNote = 'E',
    SixteenthNote = 'X',
    Flag = 'J',

    DoubleWholeStop = 0xE3,
    WholeStop = 0xB7,
    HalfStop = 0xB7,
    QuarterStop = 0xCE,
    EightStop = 0xC5,
    SixteenthStop = 0xA8,
    ThirtySecondStop = 0xF4,
    SixtyFourthStop = 0xE5,
};
