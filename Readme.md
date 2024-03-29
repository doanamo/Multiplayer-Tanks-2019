Multiplayer Tanks
====
Multiplayer game engine written from almost scratch, using only SFML library as the base for rendering and multiplatform socket wrapper. Engine supports host and client server architecture.

![](Screenshot.png)

Notable features:
* Multithreaded socket/packet processing (split between Socket/Context/Backend interfaces).
* Protocol for efficiently sending unreliable and reliable packets over UDP with minimal latency (utilizes acknowledgments and redundancy), including support for ordered packets.
* Tested with Clumsy to ensure that implementation is resiliation to packet loss, corruption and delays.
* Object replication system from server to all connected clients (with events such as creation, destruction and update).
* Host and client server architecture where player can connect at any point of the game (game state can also be saved to file).
* Serialization that can write entire game state to buffer and transfer it to connecting player.
* Custom RTTI solution for instancing arbitrary game objects from integer identifier.
* Written to compile under Linux (not actually tested if it runs).

Repository structure
====
* *Build/* - Directory where build configuration is stored and where building takes process.
* *Source/* - Directory with all of our custom written source code.
* *External/* - Directory where third party libraries and external dependencies are stored.
* *Deploy/* - Directory where compiled binaries and exported assets will be stored, ready for shipping.
* *Assets/* - Asset files that have not been exported yet to cooked format and which will not be shipped.

Building project
====
1. Install latest stable CMake release from [the official page](https://cmake.org/download/)
2. Clone our repository using Git and run *Build\CMake-VS2017.bat* file
3. Wait for Visual Studio to open and then press F5

Adding new source files
====
1. Create new source file(s) in *Source* directory
2. Open *CMakeLists.txt* in the main project directory (can be opened from Visual Studio)
3. Append path to new source file(s) to *SOURCE_FILES* variable
4. Either run *Build\CMake-VS2017.bat* again or build *CMake/ZERO_CHECK* project in Visual Studio
5. Added files should now appear in Visual Studio's project structure

Credits
====
* Piotr Doan - Game engine and networking code
* Jeremiasz Kacprzak - Tank and tile sprites
* Damian Pacholak - Design and map/camera coding
* Tomasz Herbrich - Soundtrack (not included)
