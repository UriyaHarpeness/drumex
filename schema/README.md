# Creating Parts and Exercises

## Part Structure

A Part defines a list of musical pieces, the schema is defined [here](part.json).

There are 2 ways to define a Part: [Custom](#Custom) and [Regular](#Regular).
A Part file can contain both ways at the same time.

#### Custom

The structure is as follows:

```text
{
  "Name": "Custom Part Example",
  "Global": {
      "Time Signature": <Time Signature>,
      "Type": "Custom",
      "Part": {
        "Definitions": {
          "s": <Note>
        }
      }
    },
  "Parts": [
    {
      "Time Signature": <Time Signature>,
      "Type": "Custom",
      "Part": {
        "Definitions": {
          "s": <Note>,
          ".": <Note>
        },
        "Use": [
          "s.....s.s..s...."
        ]
      }
    }
  ]
}
```

The different sections explanation:
* **Name** - The name of the Part.
* **Global** - A global section, the different attributes that a Part requires can be defined there, except for the
    `Use` attribute, if a specific Part has a definition for any of these attributes, it overrides the attribute's 
    global definition.
* **Parts** - The Parts that the Part file defines, a list of separate Parts definitions, each Part has the following
    attributes, which if are not defined will be taken from the `Global` section, except for the `Use` attribute which
    must be defined per definition.
    * **Time Signature** - The [time signature](https://en.wikipedia.org/wiki/Time_signature) of the Part, written in a
        form of list with two integers.
    * **Type** - The type of definition of the Part, this example is for the "Custom" type.
    * **Part** - Contains the actual definition of notes of the part, contains the following attributes:
        * **Definitions** - A mapping between a character and a [Notation](#Notation) definition.
        * **Use** - The use of the defined notes, a list of strings that contain the characters that are mapped to a
            note. Besides the defined characters, the symbol `|` can be used for readability, and when used as the last
            character of a string, defines that the time of the piece continues, if not used, all the different strings
            until the first occurrence of it at the end start at the same time, for example:
            ```json
            {
              "Definitions": {
               "s": [
                 "Snare",
                 [
                   1,
                   16
                 ]
               ],
               ".": [
                 "Rest",
                 [
                   1,
                   16
                 ]
               ]
             },
             "Use": [
               "s.....s.s..s....",
               "s..s...s..s...s.|",
               "s..........s...."
             ]
            }
            ```
            The first two lines are played at the same time, while the third is played after and ends the piece.

#### Regular

The structure is as follows:

```text
{
  "Name": "Regular Part Example",
  "Global": {
      "Time Signature": <Time Signature>,
      "Type": "Regular"
    },
  "Parts": [
    {
      "Time Signature": <Time Signature>,
      "Type": "Regular",
      "Part": [
        [
          <Note>,
          <Note>
        ],
        [
          <Note>
        ]
      ]
    }
  ]
}
```

The definitions are same as in the [Custom](#Custom) Part, the only difference is in the different Parts, specifically
in the `Part` attribute.

While in the custom Part, it contains the `Definitions` attribute which defines the usable notes, and the `Use`
attribute which uses them, in a "Regular" Part definition it is not so.

* **Parts**
    * **Type** - The type of definition of the Part, this example is for the "Regular" type.
    * **Part** - Contains the actual definition of notes of the part, contains a list of lists of
        [Notations](#Notation). Each item in the main list contains a list with at least on note, all the notes in the
        same sublist must have the same length, and the notes within in are played at the same time.
        For example:
        ```json
        [
          [
            [
              "Bass",
              [
                1,
                8
              ]
            ],
            [
              "HiHat",
              [
                1,
                8
              ],
              [
                "Loose"
              ]
            ]
          ],
          [
            [
              "Snare",
              [
                7,
                8
              ],
              [
                "Rimshot"
              ]
            ]
          ]
        ]
        ```
        First for 1 eighth the bass drum is played with a loose hihat, followed by a 7/8 rimshot on the snare drum.

---

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

> **Note:** Using the instrument Rest means that no note will be played, since it is a
[rest](https://en.wikipedia.org/wiki/Rest_(music)).

#### Length

**The duration of the note.**

Each note has a [duration](https://en.wikipedia.org/wiki/Duration_(music)), that duration is represented in this project
using a positive fraction, the duration of notes without any dots or tuplets, meaning it's natural duration - is always
a power of two, for example 2/1, 1/1, 1/4, or 1/32.

#### Modifiers

**Modifications for the note.**

A note can have any number of modifiers, the modifiers change the way a note is played, for example a ghost note, is
played quietly.

The modifiers supported are:
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

#### Examples

```json
["Snare", [1, 4], ["Right", "Ghost"]]
```
A quarter note, played on the snare drum, using the right hand as a ghost note.

```json
["Rest", [1, 1]]
```
A whole note rest.

```json
["Bass", [3, 4]]
```
A 3/4 length note played on the bass drum, although notes only have a natural length that is a power of 2, the notes can
be defined with the desired length, and will later on be translated to valid notes, for example a dotted eighth note.

---