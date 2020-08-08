# HEXVOID

***This is work in progress.***  
*It is not completed, it has no goal and no deadline*

Hexvoid is a very simple game, composed of colored hexagons on hexagonal grid. 

It's made in c++17 using the most basic tools available - SDL2. Why SDL2? Strictly to learn the very basics of game engines and design the hard way. Engine of the game is being designed to be reusable for other games of similar type.  

## Goal of the game

Goal of the game is to join same color hexagons together using rotation. When all the neighbours of a given hexagon are the same color, score is increased and hexagons are randomized. To make it a bit difficult, player has limited number of moves.  

### Controls

Left click - rotate clockwise  
Right click - rotate counterclockwise  
Esc - Menu

## Clone, compile and run

This guide follows step-by-step installation of SDL SDK, repository clone and build process. This should work on any linux debian system. Made and tested on Ubuntu 20.04.

### Prerequisites

Follow these steps if you're not sure if you have everything set up on linux.

```bash
sudo apt update
sudo apt upgrade
sudo apt install git cmake build-essential
```

### SDK Install

```bash
sudo apt install libsdl2-dev
sudo apt install libsdl2-gfx-dev
sudo apt install libsdl2-ttf-dev
```

### Clone and make

```bash
git clone https://github.com/acerne/hexvoid.git
cd hexvoid
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
```

### Run

```bash
./hexvoid
```