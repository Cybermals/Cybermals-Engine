# Cybermals-Engine
A lightweight high-performance game engine and application framework. 


## Features
### CybObjects
* reference-counted objects
    * optional thread-safety
* linked lists
    * optional thread-safety
    * supports insert, remove, and get
    * supports arbitrary element size
* vector collections
    * optional thread-safety
    * supports insert, remove, and get
    * supports arbitrary element size
* queues
    * optional thread-safety
    * supports enqueue, dequeue, is_full, and is_empty
    * supports arbitrary element size
    
### CybUI
* depends on CybObjects
* grid-based widgets
    * grid
    * label
    * button
        * supports both push and toggle modes
    * text box
        * supports both multi-line and read-only modes
        * supports variable-width fonts
        * supports caret placement via clicking
        * supports insertion and removal of both text and entire lines
        * does not currently support selection of blocks of text
        * does not currently support cut/copy/paste
    * list box
        * supports both single and multi-select modes
    * progress bar
        * supports max and current values
    * supports creation of custom widgets
* UI event subsystem
    * translates normal SDL2 events into UI events as needed 
    (ex. SDL_MOUSEMOTION -> CYB_UI_MOUSEMOTION)
    * translated events indicate what widget received the event
* cached resource management
    * textures
    * fonts
* XML UI definition files
    * build simple to complex user interfaces with a single self-contained UI
    definition file
    * UI definitions can also be stored as strings within your app or loaded from
    any source supported by the SDL_RWops structure
    
    
## CybMath
* 2D, 3D, and 4D vectors
    * supports addition, subtraction, multiplication, and division
* 4 x 4 matrices
    * supports identity, translation, rotation, scaling, and projection matrix
    generation
    * supports multiplication
* axis-aligned bounding boxes
    * supports collision detection with points and other boxes
    * supports generation of a box from geometry data
    * supports generation of geometry data from a box
    * supports rotation
* sphere bounding volumes
    * supports collision detection with points and other spheres
    
    
## Supported Platforms
* Windows
* Android


## License
Cybermals-Engine is under the MIT license, however the 3rd party libraries it uses
are under their own respective licenses. See the README, LICENSE, and/or COPYING files
in the "deps/{platform}/{arch}/{library}/bin" folders for details.