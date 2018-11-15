# a game

a simple game built around a 120x30 console.

required ANSI escape sequences to be supported. 

graphics development takes place in the `gfx` branch.

build via `g++ audio.cpp console.cpp display.cpp draw.cpp game.cpp interact.cpp main.cpp -lsfml-graphics-d -lsfml-audio-d -lsfml-network-d -lsfml-window-d -lsfml-system-d -Wall -std=c++14 -g -fexpensive-optimizations -O3 -std=c++14 -IC:\sfml\SFML-2.4.2\include -LC:\sfml\SFML-2.4.2\lib` 

