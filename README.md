# ASCII-MapEditor
Design tool for creating game maps using **ASCII chars**.
>The purpose of this project is to get a basic understanding of simple programming concepts of game development like creating maps, checking input from the user, saving, loading and other concepts that aren't tied to graphics.

## Features
- Create worlds using just ASCII characters
- Saving and loading maps
- Insert transitions between different maps
- Use different tile types
- Draw tile information
- Action Keys
- Collision detection

## Controls
Input | Description
------------ | -------------
A | This prompts the user to set the cursor tile's action key.
L | This prompts the user to load a file.
S | This prompts the user to save the file.
I | This selects the item tiles (the character 'I' is displayed to represent an item).
N | This selects the NPC tiles (like citizens, shopkeepers, etc...) with smiley faces.
M | This selects the monster tiles, which can be any character to represent them since these characters are defined in the Monsters.txt file in the "image:" spot.
T | This inserts a transition tile and prompts the user to enter the filename of the map to transition to.
Space_Bar | This cycles through the different tiles, once that category is chosen.
Left_Mouse_Button | This inserts a tile into the map.
Right_Mouse_Button | This first deletes the current cursor tile, and then deletes tiles under the cursor on the map. If you want to delete a monster tile, you need to have the monster tiles category chosen (M key). It will not delete a tile if that tile is not part of the chosen category. If you are in TILE_TYPE mode and have no attached cursor it will copy a tile
SHIFT | This allows the user to place many exits before switching over to the destination map. If you hold down shift and click on normal tiles in TILE_TYPE mode it will set them solid.
ESC | Quits the application.

## External Data Files
File Name | Format
------------ | -------------
Items.txt | "%s > life: %d strength: %d protection: %d"
Monsters.txt | "%s > image: %c life: %d strength: %d speed: %d"
Npcs.txt | "%s > life: %d strength: %d speed: %d message: %s"

## Task List
- [x] Finish base tutorial
- [ ] Convert project to C++11 standard
- [ ] Add better visual interface for tile selection and information
- [ ] Add ability to copy-paste a selection of the map
- [ ] Add multiple tile selection ability for adding/removing more tiles at once
- [ ] Add support for different size maps
- [ ] Support for attaching maps to other maps (scrolling map)
- [ ] Support for older maps by adding versioning to map files

## Versions
Version | Description
------------ | -------------
0.5 | Added action keys, collision detection and tile copying.
0.4 | Added map transition functionality.
0.3 | Added map loading/saving.
0.2 | Added insertTile(), deleteTile(), drawTileInfo(), setTileInfo() and support for reading external data files for object creation.
0.1 | Added Map & Tile classes, keyboard/mouse input checking, drawing default map.



> The base of this application was developed using a tutorial from [GameTutorials][gametutorials].

-----------
[gametutorials]:https://github.com/gametutorials
