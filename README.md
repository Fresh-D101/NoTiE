# NoTiE
 
## About
No Title Theme Park (NTTP) is a game concept for a Theme Park Tycoon game for the PC. Inspired by the old classic RollerCoaster Tycoon games by Chris Sawyer, it revolves around a 2D isometric grid on which the player can place objects like shops, pathways, rides, and scenery to build their own unique theme parks.

Unlike other modern implementations of Theme Park Tycoon games, NTTP does not use any 3D Objects but embraces the unique look of isometric sprites. Instead of using available commercial game engines like Unity or Unreal Engine, NTTP is being developed from scratch with C++ and the help of a few open source libraries such as Simple and Fast Media Library (SFML), DearImGui, and spdlog. Due to the time limitations and the fact, that this is a solo project, the majority of the project work done in this semester has been the implementation of the underlying custom engine, called No Title Engine (NoTiE). With NTTP in mind, NoTiE is set of systems specifically designed for isometric, sprite based games.

## Highlights

 - Data-Oriented Entity Component System making use of data locality to
   avoid CPU cache misses and therefore increasing performance
  - Isometric Engine handling the map, tile detection and selection, and
   coordinate conversions
  - Simple Map Editor enabling to change the tiles on the map as well as
   saving and loading the maps back into the engine
  - Texture Manager to ensure sprites are only once stored on the GPU and
   can be reused for multiple entities
 -  Engine and Application Logging using [spdlog](https://github.com/gabime/spdlog)
 -  User Interface using [Dear ImGUI](https://github.com/TheCherno/imgui)
