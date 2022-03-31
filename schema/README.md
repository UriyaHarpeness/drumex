# Creating Parts and Exercises

Following is a description for how to create Parts and Exercises, the way for the user to interact with DrumEX.

> It is recommended for clarity to refer to the [examples](../resources/examples).

## Part Structure

A Part defines a list of musical pieces, the schema is defined [here](part.json).

There are 2 ways to define a Part: [Custom](#Custom) and [Regular](#Regular). A Part file can contain both ways at the
same time.

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
  `Use` attribute, if a specific Part has a definition for any of these attributes, it overrides the attribute's global
  definition.
* **Parts** - The Parts that the Part file defines, a list of separate Parts definitions, each Part has the following
  attributes, which if are not defined will be taken from the `Global` section, except for the `Use` attribute which
  must be defined per definition:
    * **Time Signature** - The [time signature](https://en.wikipedia.org/wiki/Time_signature) of the Part, written in a
      form of list with two integers.
    * **Type** - The type of definition of the Part, this example is for the "Custom" type.
    * **Part** - Contains the actual definition of notes of the part, contains the following attributes:
        * **Definitions** - A mapping between a character and a [Notation](#Notation) definition.
        * **Use** - The use of the defined notes, a list of strings that contain the characters that are mapped to a
          note.
          **Special characters**:
            * `|` - Can be used for readability, and when used as the last character of a string, defines that the time
              of the piece continues, if not used, all the different strings until the first occurrence of it at the end
              start at the same time, or example:
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
            * `(` and `)` - Can be used for grouping several notes with the same length in the same location, for
              example:
                ```json
                {
                  "Definitions": {
                   "s": [
                     "Snare",
                     [
                       1,
                       4
                     ]
                   ],
                   "b": [
                     "Bass",
                     [
                       1,
                       4
                     ]
                   ],
                   ".": [
                     "Rest",
                     [
                       1,
                       4
                     ]
                   ]
                 },
                 "Use": [
                   "(sb).s."
                 ]
                }
                ```
              The first 1/4 of the piece will play both the snare and the bass drum, while at 3/4 it will only play the
              snare drum.

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
      same sublist must have the same length, and the notes within in are played at the same time. For example:
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

## Exercise Structure

An Exercise defines variations of and relation between musical pieces, written as [Parts](#Part-Structure), the schema
is defined [here](exercise.json).

#### General Structure

The structure is as follows:

```text
{
  "Name": "Exercise Example",
  "Variations": [
    {
      "Parts": [
        {
          "Part": <Part path>,
          "Indexes": [
            <Index>,
            <Index>
          ],
          "Apply": []
        },
        {
          "Part": "<Part path>,
          "Indexes": [
            <Index>
          ],
          "Apply": [
            <Variation>,
            <Variation>
          ]
        }
      ],
      "Time Signature": <Time Signature>,
      "Combination": <Combination>
    }
  ]
}
```

The different sections' explanation:

* **Name** - The name of the Exercise.
* **Variations** - The Variations that the Exercise file defines, a list of separate Exercises definitions, each
  Exercise has the following attributes:
    * **Parts** - The Parts used by the Variation, a list of definitions each containing the following attributes:
        * **Part** - The path of the used Part.
        * **Indexes** - A list of the indexes of the different pieces defined in the Part file to be used by the
          exercise, can alternatively be set to "All" in order to include all the pieces defined in that Part file.
        * **Apply** - A list of variations to apply to the chosen musical pieces, the Variations are explained further
          in the [Variations](#Variations) section.
    * **Time Signature** - The [time signature](https://en.wikipedia.org/wiki/Time_signature) of the Exercise, written
      in a form of list with two integers, if this field is not supplied, it will be derived from the first use Part's
      time signature.
    * **Combination** - The configurations by which the different Parts will be combined, there are two combinations
      options: "Merge" and "Alternate", the way they are tell apart is with the `Type` field.
        * **Type** - The type of the combination, there are two types:
            * Merge - Play the parts at the same time, merge them to a single piece.
            * Alternate - Alternate between the Parts, creates a piece with one Part played at a time.
        * **Arguments** - If the combination type is "Alternate", this field is expected, it contains the field
          `Indexes` which is explained below.
            * **Indexes** - The indexes of the Parts used in the exercise, in the order of alternation desired, each
              index adds a length of the time signature of the matching Part, the Parts will be stretched to fit the
              length at which the cycle will be complete for all of them together, for example:

              `[0]` - The first defined Part will always be played, same as having it as the only defined Part.

              `[1, 0]` - Alternating between the second and the first Parts until they're both finished.

              `[0, 1, 1, 1, 2]` - One bar of the first Part, followed by three bars of the second Part, and finished
              with a bar one the third Part.

#### Variations

Each Variation defined an action to apply to a musical piece, each Variation has the attributes `Name` and optionally
`Arguments`, below is the full list and explanation of the different Variations, the title of the variation is the name
of that Variations, that needs to be in the `Name` attribute of the Variation, the attributes listed below the name are
to be put in the `Argument` field, in the following format:

```text
{
  "Name": <Variation Name>,
  "Arguments": {
    <First Argument Name>: <First Value>,
    <Second Argument Name>: <Second Value>,
  }
}
```

##### Change Note

Change notes that match the instrument of modifiers supplied.

* **Match** - The arguments by which the notes will be checked for a match, if any of them are missing, a match will be
  based on the other argument, if both are missing, all the notes will match.
    * **Instruments** - A list of instruments that if a note is played on any of them it will be matched.
    * **Modifiers** - A list of modifiers, that if a note contains on any of them it will be matched.
* **Apply** - The change to be done on the matched notes, if any of the arguments are missing, it will not be overridden
  with that argument.
    * **Instrument** - The instrument to set for the note to be played on.
    * **Modifiers** - The modifiers to set for the note to be played with.

##### Tuplet

Change the piece to be played as tuplets.

* **From** - The base units value.
* **To** - The destination units value.
* **Units** - The units on which the to and from are counted with, written as a fraction, a list containing two
  integers.

For example:

```json
{
  "From": 4,
  "To": 5,
  "Units": [
    1,
    16
  ]
}
```

Will convert each group of 4 1/16 (1/4) to quintuples (1/20 in this case).

##### Double

Double matching notes, with carry or without, with a given distance.

* **Distance** - The distance at which to double the matching notes, written as a fraction, a list containing two
  integers.
* **Match** - Same as the `Match` argument in [Change Note](#Change-Note).
* **Carry** - A flag that denotes whether to carry doubled notes of not, the behavior when a double meets a matching
  note changes based on its value (for the examples below `m` is a matched note, `.` is a non-matching note, `d` is the
  double of the matching notes):
    * `false` - If a note is matched, but `Disntance` from it another note is matched, the first note match will be
      ignored, for example:
        ```text
        .m..mm..m.m..mmm....m ->
        .md.mmd.mdmd.mmmd...m
        ```
    * `true` - If a note is matched, but `Disntance` from it another note is matched, the match will carry on, and at
      the end of the matching note chain, until the next matching, the number of matches with be multiplied, for
      example:
        ```text
        .m..mm..m.m..mmm....m ->
        .md.mmddmdmd.mmmddd.m
        ```

##### Fill

Fills the piece with a given spacing where there are no notes matching.

* **Distance** - The spacing between the notes, written as a fraction, a list containing two integers.
* **Match** - Same as the `Match` argument in [Change Note](#Change-Note).
* **Apply** - Same as the `Apply` argument in [Change Note](#Change-Note), except the fields are required.

##### Sticking

Change the sticking on the notes (right, left).

* **Sticking** - The sticking for the piece, each played note group gets the specified sticking, it is a string that
  contains the characters `R`, `L`, `.`, `0`, and `x`. Here is what each of these mean:

  `R` - Play with the right hand.

  `L` - Play with the left hand.

  `.` - Keep the current sticking.

  `0` - Drop the sticking.

  `x` - Reverse the sticking.

##### Stretch Sticking

Stretch the notes sticking, continue the same sticking of the previous note until another sticking is met, gives the
notes without sticking at the start of the piece the opposite of the first found sticking.

##### Fit

Fit the piece to a given length (repeats and trims).

* **Length** - The length to fit the piece into.

##### Scale

Scales a piece with a given ratio.

* **Ratio** - The ratio to scale with, a fraction, for example for `[2, 1]`, all the notes will be twice as long as
  their original length.

##### Filter

Filter notes, drop notes that does not match the arguments, or the opposite.

* **Match** - A list containing the same object as the `Match` argument in [Change Note](#Change-Note).
* **Invert** - Optional flag, is exists and set to `true`, inverts the notes matching.

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
lines usually refer to different instruments. In drums and percussion notation in general, different instruments can
even have different
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
[
  "Snare",
  [
    1,
    4
  ],
  [
    "Right",
    "Ghost"
  ]
]
```

A quarter note, played on the snare drum, using the right hand as a ghost note.

```json
[
  "Rest",
  [
    1,
    1
  ]
]
```

A whole note rest.

```json
[
  "Bass",
  [
    3,
    4
  ]
]
```

A 3/4 length note played on the bass drum, although notes only have a natural length that is a power of 2, the notes can
be defined with the desired length, and will later on be translated to valid notes, for example a dotted half note.

---