
# 2D Component-Based Engine

This project contains a 2D component-based game engine in C++. This engine also features a remake of the Sega game Pengo, playable [here](https://stoan24.github.io/Prog4-Engine/).

[![Build Status](https://github.com/Stoan24/Prog4-Engine/actions/workflows/cmake.yml/badge.svg)](https://github.com/Stoan24/cmake/actions)
[![Build Status](https://github.com/Stoan24/Prog4-Engine/actions/workflows/emscripten.yml/badge.svg)](https://github.com/Stoan24/emscripten/actions)

# Features

The engine is built on a Component-Based Architecture, allowing for highly decoupled and reusable game logic. Key features include:
- Scene Graph & GameObjects: Hierarchical structure for managing game entities.
- Component System: Logic is split into modular components (Transform, SpriteRenderer, Collider, Controller, etc.).
- Input Management: Support for Keyboard and Controller (XInput) using the Command Pattern.
- Resource Manager: Centralized loading for textures, fonts, and sounds.
- Sound System: A thread-safe service locator for handling audio playback via SDL_Mixer.
- Game Loop: A fixed-timestamp update loop to ensure consistent physics and movement regardless of frame rate.

# Pengo

The primary goal of this project is to recreate the gameplay mechanics of Pengo using a custom-built engine architecture.
- Grid-based Movement: Logic for navigating the icy maze.
- Ice Block Physics: Mechanics for pushing, sliding, and crushing enemies with ice blocks.
- AI Behavior: Pathfinding and state logic for the "Sno-Bees."
- Sound & Animation: Retro-accurate audio triggers and sprite animations.

# Minigin

This project is built upon Minigin, a lightweight starting point designed for the Programming 4 course at Digital Arts & Entertainment (DAE). Minigin provides a barebones setup for:
- SDL3 for window management, rendering, and input.
- GLM for optimized 2D/3D mathematics.
- A foundational structure to implement Game Programming Patterns as described by Robert Nystrom.
- CMake Workflow: Builds the project in Debug/Release for Windows on every push to ensure code stability.
- Emscripten Workflow: Automatically compiles the web version and deploys it to [GitHub Pages](https://stoan24.github.io/Prog4-Engine/).

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Use

Get the source from this project, or since students need to have their work on github too, they can use this repository as a template. Hit the "Use this template" button on the top right corner of the github page of this project.

## Windows version

Either
- Open the root folder in Visual Studio 2026; this will be recognized as a cmake project.
  
Or
- Install CMake 
- Install CMake and CMake Tools extensions in Visual Code
- Open the root folder in Visual Code,  this will be recognized as a cmake project.

Or
- Use whatever editor you like :)

## Emscripten (web) version

### On windows

For installing all of the needed tools on Windows I recommend using [Chocolatey](https://chocolatey.org/). You can then run the following in a terminal to install what is needed:

    choco install -y cmake
    choco install -y emscripten
    choco install -y ninja
    choco install -y python

In a terminal, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake ..
    emmake ninja

To be able to see the webpage you can start a python webserver in the build_web folder

    python -m http.server

Then browse to http://localhost:8000 and you're good to go.

### On OSX

On Mac you can use homebrew

    brew install cmake
    brew install emscripten
    brew install python

In a terminal on OSX, navigate to the root folder. Run this: 

    mkdir build_web
    cd build_web
    emcmake cmake .. -DCMAKE_OSX_ARCHITECTURES=""
    emmake make

To be able to see the webpage you can start a python webserver in the build_web folder

    python3 -m http.server

Then browse to http://localhost:8000 and you're good to go.

# Credits & Resources

- Engine Author: [Stoan24]
- Minigin Base: Created for DAE Programming 4.
- Book: Game Programming Patterns by Robert Nystrom.
- Original Game: [Pengo](https://en.wikipedia.org/wiki/Pengo_(video_game)) (1982) by SEGA / Coreland.
