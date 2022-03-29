import json
import pathlib

from jsonschema import RefResolver, Draft7Validator

schemas = {path.name: json.load(path.open()) for path in pathlib.Path('schema').glob('*.json')}

part_validator = Draft7Validator(schemas['part.json'],
                                 resolver=RefResolver.from_schema(schemas['part.json'], store=schemas))
exercise_validator = Draft7Validator(schemas['exercise.json'],
                                     resolver=RefResolver.from_schema(schemas['exercise.json'], store=schemas))

VALIDATORS = {'.part': ('Part', Draft7Validator(schemas['part.json'],
                                                resolver=RefResolver.from_schema(schemas['part.json'], store=schemas))),
              '.exercise': ('Exercise', Draft7Validator(schemas['exercise.json'],
                                                        resolver=RefResolver.from_schema(schemas['exercise.json'],
                                                                                         store=schemas)))}


def main():
    for path in sorted(pathlib.Path('resources').glob('**/*.json')):
        type_, validator = VALIDATORS[path.suffixes[0]]
        print(f'Validating {type_}: {path}')
        validator.validate(json.load(path.open()))


if __name__ == '__main__':
    main()
