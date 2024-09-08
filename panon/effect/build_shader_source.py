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


def hex2vec4(value):
    assert value.startswith('#')
    value = value[1:]
    nums = [int(value[i:i + 2], base=16) / 256 for i in range(0, len(value), 2)]
    if len(nums) < 4:
        nums.insert(0, 1)
    assert len(nums) == 4
    return "vec4(%f,%f,%f,%f)" % tuple(nums[1:] + nums[:1])


def format_value(type_, value):
    if type_ == 'int':
        return f'int({value})'
    if type_ == 'double':
        return f'float({value})'
    if type_ == 'float':
        return f'float({value})'
    if type_ == 'bool':
        return f'bool({value})'
    if type_ == 'color':
        return hex2vec4(value)
    return value


def build_source(files, main_file: Path, meta_file: Path = None, effect_arguments=None):
    if not main_file.exists():
        return ''
    arguments_map = {}
    # If meta_file exists, construct a key-value map to store arguments' names and values.
    if meta_file is not None and meta_file.exists():
        meta = json.loads(meta_file.read_bytes())
        arguments_map = {arg['name']: format_value(arg['type'], value) for arg, value in zip(meta['arguments'], effect_arguments)}

    source = ""
    for path in files:
        if path == main_file:
            for line in list(read_file_lines(path)):
                lst = line.split()
                if len(lst) >= 3:
                    # Search for used arguments(start with $) in macro definitions.
                    if lst[0] == '#define' and lst[2].startswith('$'):
                        key = lst[2][1:]
                        # Raise an error when the value of an argument is not found.
                        if key not in arguments_map:
                            json.dump({"error_code": 1}, sys.stdout)
                            sys.exit()
                        lst[2] = arguments_map[key]
                        line = ' '.join(lst) + '\n'
                source += line
        else:
            source += read_file(path)
    return source


def texture_uri(path: Path):
    if path.exists():
        return str(path.absolute())
    return ''

def build_qsb(shader_source: str) -> str:
    frag, frag_path = tempfile.mkstemp(dir=tmp_dir, suffix='.frag')
    os.write(frag, shader_source.encode())
    os.close(frag)
    
    # Find qsb binary
    paths = ["/usr/lib64/qt6/bin/qsb", "/usr/lib/qt6/bin/qsb"]
    qsb_bin_path = ""
    for path in paths:
        if os.path.exists(path):
            qsb_bin_path = path
    if qsb_bin_path == "":
        raise Exception("qsb not found")
    
    qsb, qsb_path = tempfile.mkstemp(dir=tmp_dir, suffix='.qsb')
    subprocess.run([qsb_bin_path, '--glsl', '150', frag_path, '-o', qsb_path], check=True)
    os.close(qsb)

    return qsb_path

applet_effect_home = effect_dirs[-1]

image_shader_path = Path(effect.path)
if not effect.name.endswith('.frag'):
    image_shader_path /= 'image.frag'

image_shader_files = [
    applet_effect_home / 'shadertoy-api-head.fsh',
    applet_effect_home / 'hsluv-glsl.fsh',
    applet_effect_home / 'utils.fsh',
    image_shader_path,
    applet_effect_home / 'shadertoy-api-foot.fsh',
]

if effect.name.endswith('.frag'):
    obj = {'image_shader': build_source(image_shader_files, image_shader_path)}
else:
    obj = {
        'image_shader':
        build_source(
            image_shader_files,
            image_shader_path,
            Path(effect.path) / 'meta.json',
            effect_arguments,
        ),
        'buffer_shader':
        build_source(
            [
                applet_effect_home / 'shadertoy-api-head.fsh',
                Path(effect.path) / 'buffer.frag',
                applet_effect_home / 'shadertoy-api-foot-buffer.fsh',
            ],
            Path(effect.path) / 'buffer.frag',
            Path(effect.path) / 'meta.json',
            effect_arguments,
        ),
        'texture':
        texture_uri(Path(effect.path) / 'texture.png'),
    }

obj['wave_buffer'] = read_file(applet_effect_home / 'wave-buffer.fsh')
obj['gldft'] = read_file(applet_effect_home / 'gldft.fsh')
obj['enable_iChannel0'] = 'iChannel0' in (read_file(image_shader_path) + (read_file(Path(effect.path) / 'buffer.frag') if (Path(effect.path) / 'buffer.frag').exists() else ""))
obj['enable_iChannel1'] = 'iChannel1' in (read_file(image_shader_path) + (read_file(Path(effect.path) / 'buffer.frag') if (Path(effect.path) / 'buffer.frag').exists() else ""))

# Create the temporary directory for the shader files.
tmp_dir = Path('/tmp/panon')
tmp_dir.mkdir(exist_ok=True)

if 'image_shader' in obj and obj['image_shader']:
    obj['image_shader'] = build_qsb(obj['image_shader'])

if 'buffer_shader' in obj and obj['buffer_shader']:
    obj['buffer_shader'] = build_qsb(obj['buffer_shader'])

json.dump(obj, sys.stdout)
logger.log('obj : %s',json.dumps(obj))
