# Project Proposal
by __Nakul Iyer__ for __CS 126__

In this project I would like to build a watered-down version of *Minecraft: Creative Mode*. These are the goals I hope to accomplish (in the order of general importance and interest):

- [ ] allow the user to manuever a camera by rotation and transformation through the keyboard and mouse, and interact with three-dimensional objects
  - [https://libcinder.org/docs/guides/opengl/part2.html](https://libcinder.org/docs/guides/opengl/part2.html) will be helpful here
- [ ] dynamically create a three-dimensional world out of blocks through an RNG seed
  - [ ] a selection of designed blocks (i.e. grass, stone, etc) and maybe tools (i.e. axe, pickaxe) that will be significantly less than the original game but enough to test and play through
  - [ ] some noise function that will create a smooth world (not full of cliffs)
- [ ] allow the user to destroy and place blocks
  - [ ] this will, of course, necessitate a save/load mechanic

In order to realistically complete this in three weeks, my minimum viable product will drop some several features of *Minecraft* including mobs, dimensions other than the overworld, and automatic structures (trees, oceans, villages, etc). The graphics will be far simpler, and several features may be conveyed through text instead of image. By the first week, I will complete the camera system (which I can test on static three-dimensional geometric figures). By this point, I should be able to perceive the depth of the object, be able to view all angles, and zoom/move just as any open world game would. By the second week, I will find a noise function, create a world-building method, and have demo blocks to test this. By this point, I should be able to see and move around the world without interacting or passing through blocks. By the third week, there will be some gravity so that the player actually lands on the ground. The player should be able to edit the world be deleting and creating blocks, and the game should save the world and the player's inventory. Users should be able to load from this save file.

If I complete the minimum viable product earlier than expected, many of the next features will be implementing things that are in the original game. In particular (and in order of perceived level-of-effort), these would be

- [ ] trees
- [ ] more interesting noise functions to allow for connected caves and bodies of water
  - [ ] connected caves
  - [ ] connected bodies of water
  - [ ] random ores in the ground
- [ ] graphics optimizations, especially only rendering the block faces that are exposed to air

Finally, if I sense that the workload is far greater than expected, I'm still interested in doing something with OpenGL's three-dimensional camera. I would like to redirect the camera feature to a *Portal* clone; because I will have to design simple levels, it will clear up the RNG-seed level generation work.
