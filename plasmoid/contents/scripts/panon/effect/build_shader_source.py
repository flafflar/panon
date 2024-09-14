import sys
import json
from pathlib import Path

from .helper import effect_dirs
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
    if e.id == effect_id:
        effect = e
        break

    if e.name == 'default':
        effect = e

def texture_uri(path: Path):
    if path.exists():
        return str(path.absolute())
    return ''

applet_effect_home = effect_dirs[-1]

obj = {
    'image_shader': str(Path(effect.path) / 'image.qsb'),
    'buffer_shader': str(Path(effect.path) / 'buffer.qsb'),
    'wave_buffer': applet_effect_home / 'wave-buffer.qsb',
    'gldft': applet_effect_home / 'gldft.qsb',
    'texture': texture_uri(Path(effect.path) / 'texture.png'),
    'enable_iChannel0': True,
    'enable_iChannel1': True,
}

json.dump(obj, sys.stdout)
