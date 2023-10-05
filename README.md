CSC 581 Homework 2: Time and Networking Foundations
====
This assignment is to explore how to represent time and the basics of constructing a multithreaded network server.

Prerequisites
----
* Ubuntu 20.04
    + Desktop system installed
    + Additional packages
        - `x11-apps`
        - `build-essential`
        - `libsfml-dev`
        - `libzmq3-dev`

Getting Started
----
1. In the root directory of this project, run `make` in command line.
    * If succeed, three executables: `single-player`, `server` and `client` should appear in the directory.
2. For section 1 and 2, run `./single-player` in a terminal which is opened from the Ubuntu desktop.
    * If succeed, a 800 x 600 window should appear and there will be a white platform and a white ball in the window.
    * Control the ball with arrow keys.
    * Press `p` to pause the game.
    * Press `s` to switch between game speeds (1x, 2x, 0.5x).
3. For section 3 to 5, run `./server` in any terminal and then run `./client` in a terminal which is opened from the Ubuntu desktop.
    * If succeed, a 800 x 600 window should appear and there will be a white platform and a white ball in the window.
    * Control the ball with arrow keys.
    * Press `p` to pause the game, however the moving platform will keep moving in the server side.
    * Press `s` to switch between game speeds (1x, 2x, 0.5x), however only the white ball is affected by client game speed.
    * Each run of `./client` will create a new white ball that can be observed on all clients, but only the corresponding window can control the ball.

Reference
----
* `asset/texture.jpg`: by [Jonathan Borbaf](https://www.pexels.com/photo/fine-sandy-dunes-in-dry-desert-5489194/)

Prerequisites
----
* Ubuntu 20.04
    + Desktop system installed
    + Additional packages
        - `x11-apps`
        - `build-essential`
        - `libsfml-dev`

Getting Started
----
1. In the root directory of this project, run `make` in command line.
    * If succeed, an executable `sfml-app` should appear in the directory.
2. Run `./sfml-app` in a terminal which is opened from the Ubuntu desktop.
    * If succeed, a 800 x 600 window should appear and a blue platform is moving inside it.
3. Control the red ball and try to stay on the blue platform.
    * Use the left and right buttons to move vertically.
    * Use space bar to jump when on a platform.
    * Use `/` button to switch the window betweem constant size mode and proportional scaling mode.
