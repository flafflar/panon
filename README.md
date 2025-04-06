# Panon
An audio visualizer widget for KDE Plasma.

![Screenshot of panon](https://raw.githubusercontent.com/wiki/flafflar/panon/Screenshots/panon.gif)

## Installation
Go to [Releases](https://github.com/flafflar/panon/releases) and download the 
latest `panon.plasmoid` file. Then, right click on your desktop, select 
*Add Widgets* > *Get New Widgets...* > *Install Widget From Local File...*, 
and select the file you downloaded.

For the widget to actually work, make sure you have installed the required
dependencies. See [below](#dependencies) for the dependencies you need based on 
your distro.

## Dependencies

On first run, the plasmoid will create a venv and install Python dependencies automatically.

### KDE Neon
```sh
sudo apt install qt6-websockets python3-pip python3-venv
```

### OpenSUSE
```sh
sudo zypper in qt6-shadertools qt6-websockets-imports
```

### Nobara
```sh
sudo dnf install qt6-qtwebsockets-devel qt6-qtshadertools
```

### Arch Linux
```sh
sudo pacman -S --asdeps qt6-shadertools qt6-websockets
```
You can also install the [AUR package](https://aur.archlinux.org/packages/plasma6-applets-panon)
directly!

If you are using another disto, please open an issue so I can include it here.

## Credits

### Main contributors
In chronological order:
| Contribution                       | Contributor                                                  |
|------------------------------------|--------------------------------------------------------------|
| Original developer                 | [rbn42](https://github.com/rbn42)                            |
| German translation                 | [NullDev](https://github.com/NLDev)                          |
| "Download New Effects" dialog      | [flying-sheep (Phillip A.)](https://github.com/flying-sheep) |
| Dutch translation                  | [Vistaus (Heimen Stoffels)](https://github.com/Vistaus)      |
| "Monitor of current device" option | [Yuannan](https://github.com/yuannan)                        |
| Port to Plasma 6                   | [flafflar](https://github.com/flafflar)                      |

### Third-party code
| Files                                                                                           | Source                                                                                           |
|-------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------|
| [source.py](panon/backend/source.py) and [spectrum.py](panon/backend/spectrum.py)               | adapted from [PyVisualizer](https://github.com/ajalt/PyVisualizer)                               |
| `hsv2rgb` in [utils.fsh](plasmoid/contents/shaders/utils.fsh)                                   | copied from [GLSL-color.md](https://gist.github.com/patriciogonzalezvivo/114c1653de9e3da6e1e3)   |