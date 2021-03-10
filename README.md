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
* grid-based widgets
    * grid
    * label
    * button
        * supports both push and toggle modes
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


## License
Cybermals-Engine is under the MIT license, however the 3rd party libraries it uses
are under their own respective licenses. See the README, LICENSE, and/or COPYING files
in the "deps/{platform}/{arch}/{library}/bin" folders for details.