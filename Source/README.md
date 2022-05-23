<h1>Source Code</h1>

Here are the files used in the project.

I used Visual Studio 2019 for development. <br>
The original upload of this project contained the Visual Studio boilerplate, but this has been removed.


<h2>Files Explained</h1>

*This is a non-exhaustive description of each file, to help with understanding the program*

<h3>Source.cpp</h3>

This is the start of the project. <br>

Here, I use polymorphism to set up the olcPixelGameEngine and establish the flow of the program. <br>

I have some variable to keep track of the state, and use switch statements to call the desired part of the program.



<h3>Setup.h</h3>

Functions from this file are called from `Source.cpp` at the start of the program. <br>

At the start of the file, I create a class called `M_Vector` that just allow me to store and access position and velocity vectors in a way that feels more fluid to me. <br>

The `M_Rect` class exists for the same reason. <br>

The `MakeRectangle()` function is essentailly a constructor method for the `M_Rect` class, though I didn't know about constructors at the time.<br>

The `PositionAndRadius` 'interface' is more like an abstract class used by `C_Pocket` and `C_Ball` to remove duplicate code.<br>

The `triangleAdj()` function is used to calculate the positions of balls in the triangle formation.


<h2>olcPixelGameEngine.h</h2>
See <a href="https://github.com/OneLoneCoder/olcPixelGameEngine">olcPixelGameEngine</a>.

