# Creating Parts and Exercises

## Part Structure

A Part defines a list of musical pieces. 

[schema](part.json)

[Notation](#Notation)

### Notation

A single notation is defined with these three components:
* [Instrument](#Instrument)
* [Length](#Length)
* [Modifiers](#Modifiers)

Here's a further explanation on each of the components:

#### Instrument

**The instrument on which the note is played.**

In music, each pitch is written in a line or between lines of itself (except when using
[accidentals](https://en.wikipedia.org/wiki/Accidental_(music)) like flat or sharp), for drum notation, the different
lines usually refer to different instruments.
In drums and percussion notation in general, different instruments can even have different
[noteheads](https://en.wikipedia.org/wiki/Notehead).

The instruments supported are:
* China
* Crash
* HiHat
* HighTom
* MidTom
* Snare
* LowTom
* FloorTom
* Bass

Other instruments that are not not mapped to a real instrument are:
* Up
* Down
* Rest

---
**Note** that using the instrument Rest means that no note will be played, since it is a
[rest](https://en.wikipedia.org/wiki/Rest_(music)).
---

#### Length

**The duration of the note.**

Each note has a [duration](https://en.wikipedia.org/wiki/Duration_(music)), that duration is represented in this project
using a positive fraction, the duration of notes without any dots or tuplets, meaning it's natural duration - is always
a power of two, for example 2/1, 1/1, 1/4, or 1/32.

#### Modifiers

**Modifications for the note.**

A note can have any number of modifiers, the modifiers change the way a note is played, for example a ghost note, is
played quietly.

* CrossStick
* Ghost
* Accent
* Dot
* Rimshot
* Flam
* Drag
* Close
* Loose
* OpenClose
* Open
* Choke
* Right
* Left