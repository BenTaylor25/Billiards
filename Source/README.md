<h1>Source Code</h1>

Here are the files used in the project.

I used Visual Studio 2019 for development. <br>
The original upload of this project contained the Visual Studio boilerplate, but this has been removed.


<h1>Files Explained</h1>

*This is a non-exhaustive description of each file, to help with understanding the program*

<h2>Source.cpp</h2>

This is the start of the project. <br>

Here, I use polymorphism to set up the olcPixelGameEngine and establish the flow of the program. <br>

I have some variable to keep track of the state, and use switch statements to call the desired part of the program.



<h2>Setup.h</h2>

Functions from this file are called from `Source.cpp` at the start of the program. <br>

At the start of the file, I create a class called `M_Vector` that just allow me to store and access position and velocity vectors in a way that feels more fluid to me. <br>

The `M_Rect` class exists for the same reason. <br>

The `MakeRectangle()` function is essentailly a constructor method for the `M_Rect` class, though I didn't know about constructors at the time.<br>

The `PositionAndRadius` 'interface' is more like an abstract class used by `C_Pocket` and `C_Ball` to remove duplicate code.<br>

The `triangleAdj()` function is used to calculate the positions of balls in the triangle formation.



<h2>Menu_Screens.h</h2>

This file contains everything to do with the menus.

Fun fact: `DrawBackground()` was initially created when I was suitability-testing the olcPixelGameEngine; I liked the design enough to include it in the final product.



<h2>Game_Loop.h</h2>

The switch stack in `Source.cpp` would be much larger if the main game loops for Pool and Snooker weren't abstracted out to this file.



<h2>Game_FrontEnd.h</h2>

This file handles drawing to the screen while Pool or Snooker is in progress.



<h2>Game_BackEnd.h</h2>

This file handles the Physics and Maths required to make the game work.



<h2>Game_Rules.h</h2>

This file keeps track of the game state: <br>
- Who's turn is it? <br>
- What colour should each player target? <br>
- What should happen when a given ball is pocketed? <br>
- Is the game over?



<h2>olcPixelGameEngine.h</h2>
See <a href="https://github.com/OneLoneCoder/olcPixelGameEngine">olcPixelGameEngine</a>.



<h2>HelpText.h</h2>

This file contains the text that will be displayed on the Help page. <br>

The text wrapping is mostly handled automatically in `Setup.h`, but there are some points where you want to manually insert line breaks (e.g. at the end of a paragraph). <br>

The '¬' character is what I use to insert manual line breaks. When the printer in `Setup.h` sees a '¬' character, it inserts a line break.



<h2>CreditsText.h</h2>

This file contains the text that will be displayed on the Credits page. <br>

Thanks again to <a href="https://github.com/OneLoneCoder">David Barr</a> and <a href="https://github.com/TheWelshEngineer">Kate Upson-Smith</a>.
