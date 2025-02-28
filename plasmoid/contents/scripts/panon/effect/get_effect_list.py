"""
This module defines the data structure of an effect,
and the way to generate unique effect identities.
The data structure is used by ConfigEffect.qml and 
build_shader_source.py.
"""
import sys
from pathlib import Path
from .helper import effect_dirs
import collections

# The data structure of an effect
Effect = collections.namedtuple('Effect', 'name id path')


def get_effects(root: Path):
    if not root.is_dir():
        return

    for f in root.iterdir():
        if any(f.glob('*.qsb')):
            yield Effect(f.name, f.name, str(f.absolute()))

def get_list():
    """
    Returns an array of all available visual effects.
    """
    return sorted([effect for effect_dir in effect_dirs for effect in get_effects(effect_dir)])


if __name__ == '__main__':
    import json
    json.dump([effect._asdict() for effect in get_list()], sys.stdout)
