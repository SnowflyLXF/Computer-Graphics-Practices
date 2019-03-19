## Overview

Creating a 3D game in OpenGL is both challenging and rewarding. I made use of OpenGL and SDL library to create a 3D adventure game that is set undersea as my final project. This game requires navigating through a 3D virtual scene to catch jellyfishes meanwhile avoid sharks to finish the game. By implementing advanced features, the overall game play quality went up as well as the look and feel of the game. We learned all the basics to designing and implementing a 3D game in OpenGL from CSCI5607 as well as some of the more advanced techniques used when creating such a game. 
My goal is to create a realistic undersea world. The major task is to make the scene realistic. I need to implement complex objects and textures in the scene as well as special effects like shadows, fog and caustic. Another obstacle is to add text to the game. Moreover, a smooth motion of the character by keyboard input and a friendly menu with a mini-map will add to a better experience.

## Game Design

1. 3D viewing and Objects

Items in the game are drawn with different shapes which are loaded from obj files(). They comprise of triangle meshes. The mesh components are vertices, edges and triangles. First read all the vertices, normals, texcoords & triangles into the allocated arrays. Then use Gouraud shading model that computes the lighting at the corners of each triangle and linearly interpolating the resulting colors for each pixel covered by the triangle. (see mesh.cpp)

![Screen Shot 2019-03-19 at 08.33.03](/Users/lixuefei/Desktop/Screen Shot 2019-03-19 at 08.33.03.png)

Patrick is the major character of the game that represents the player. The game uses a first-person view, so you can only see the back of him.
Jellyfish is the bonus(see Jellyfish.cpp). They are generated randomly. A new jellyfish is added every 3s in random position at the same height, and falling at a certain speed, it disappears beneath the ground(see JellyManager.cpp). Notice that jellyfish is translucent, to create the effect, I use blend function of OpenGL to decrease the alpha value of the object to allow some light to travel through jellyfishes

![Screen Shot 2019-03-19 at 08.34.11](/Users/lixuefei/Desktop/Screen Shot 2019-03-19 at 08.34.11.png)

Shark is the villian, there are 3 sharks in total moving towards you at a certain speed(see Enemy.cpp, EnemyManager.cpp).

![Screen Shot 2019-03-19 at 08.34.23](/Users/lixuefei/Desktop/Screen Shot 2019-03-19 at 08.34.23.png)

Transformation functions including glRotatef and glTranslatef are applied to each object.

2. Camera

The camera in the game is defined with the gluLookAt function.

3. 

## Installation

Download the files and run the makefile inside the project folder in Linux.

### Dependencies

Install the following dependencies:

libsdl2-dev

libsdl2-ttf-dev

libsdl2-image-dev

## Control

Key w: Move forward

Key s: Move backward

Key q: Move upward

Key e: Move downward

Key a: Turn left

Key d: Turn right

ESC: Terminate the game

SPACE: Accelerate

F1: Help   F2: switch perspective  F3: Headlight    F4: Reflection   F5: Shadow

### Collision Detection

Each object is

bounded by a rectangular box--bounding box(in Mesh.cpp ComputeBoundingBox). A collision is detected when the volumes of the two objects’

boxes intercept one another. The condition for a collision is: for each dimension (i.e. x,y,z),

there is an overlap between the space occupied by object 1 and object 2 in that dimension. Once you collide with a jellyfish, you will gain 10 points, Once you collide with a shark, game over.