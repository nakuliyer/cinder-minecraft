[![docs](https://img.shields.io/badge/docs-yes-brightgreen)](docs/README.md)
# Cinder Minecraft

**Author**: 
Nakul Iyer - [nakuli2@illinois.edu](mailto:nakuli2@illinois.edu)

## Overview
![](docs_assets/demo.gif) <br/>
** Note: graphics are severely scaled-down for demonstration

Cinder Minecraft is a very basic clone of the popular video game, *Minecraft*. In this game, you can explore a randomly-generated, infinitely long terrain; you can interact with this world by breaking and placing grass blocks!

### Notable Features
* The game stores blocks in "chunks" (set to ~216 blocks by default). At any point in time, the player's chunk and all 26 other adjacent chunks are in view.
* All games initialize to a random seed. All blocks destroyed and placed by a player are saved in any instance of the game, throughout any number of chunk movements.

## Dependencies
** This app has only been tested on Mac OS Big Sur.
* [gcc] or [clang]
* [cmake]
* [Cinder](https://libcinder.org)
* [catch2](https://github.com/catchorg/Catch2)
* [FastNoise](https://github.com/Auburn/FastNoise)

## Quick Start
First, download [Cinder](https://libcinder.org) and clone this repository into the `my-project` folder of Cinder.

### MacOS
```
$ mkdir build
$ cd build/
$ cmake ..
$ make
$ open Debug
```
The app will be present in the opened finder window. It may be moved anywhere inside the folders corresponding to this git repository.

## Gameplay
| Key           | Action                           |
|---------------|----------------------------------|
| `w`           | Move forwards                    |
| `s`           | Move backwards                   |
| `a`           | Move leftwards                   |
| `d`           | Move rightwards                  |
| `SPACE`       | Jump                             |
| `q`           | Delete the block at the cursor   |
| `e`           | Place a block at the cursor      |
| `UP`          | Switching blocks for placement   |
| `DOWN`        | Switching blocks for placement   |

| Mouse/Pad     | Action                           |
|---------------|----------------------------------|
| `Mouse Wheel` | Pan the screen                   |

## Citations
* All credit for the concept of this game belongs to Mojang Studios, and the original game may be [downloaded here](https://www.minecraft.net/en-us)
* Texture used for grass: [https://www.html5gamedevs.com/uploads/monthly_2016_03/1356119798126.thumb.png.102fb982bde3cd0b4b3ce3bdbe98082a.png](https://www.html5gamedevs.com/uploads/monthly_2016_03/1356119798126.thumb.png.102fb982bde3cd0b4b3ce3bdbe98082a.png)
* Texture used for stone: [http://minecrafttemplate.blogspot.com/2012/05/minecraft-seamless-background-images.html](http://minecrafttemplate.blogspot.com/2012/05/minecraft-seamless-background-images.html)
* Icons are from: [https://minecraft.gamepedia.com](https://minecraft.gamepedia.com)
