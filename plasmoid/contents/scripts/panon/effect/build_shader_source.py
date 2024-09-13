import os
import sys
import json
import tempfile
import subprocess
from pathlib import Path

from docopt import docopt

from .helper import effect_dirs, read_file, read_file_lines
from . import get_effect_list

import json, base64

from .. import logger

import sys
logger.log('argv: %s', sys.argv[1:])

effect_id, effect_arguments = json.loads(base64.b64decode(sys.argv[1]))
logger.log('effect_id: %s', effect_id)
logger.log('effect_arguments: %s', effect_arguments)

effect_list = get_effect_list.get_list()
effect = None

# Set a default effect.
for e in effect_list:
    if e.name == 'default':
        effect = e

for e in effect_list:
    if e.id == effect_id:
        effect = e

obj = {
    'image_shader': str(Path(effect.path) / 'image.qsb'),
    'buffer_shader': str(Path(effect.path) / 'buffer.qsb'),
    'wave_buffer': 'TODO',
    'gldft': 'TODO',
    'texture': 'TODO',
    'enable_iChannel0': True,
    'enable_iChannel1': True,
}

json.dump(obj, sys.stdout)
