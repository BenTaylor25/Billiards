Here are the files used in the project.

I used Visual Studio 2019 for development. <br>
The original upload of this project contained the Visual Studio boilerplate, but this has been removed.

<h2>Files Explained</h1>
<h3>Source.cpp</h3>
This is the start of the project. <br>
Here, I use polymorphism to set up the olcPixelGameEngine and establish the flow of the program. <br>
I have some variable to keep track of the state, and use switch statements to call the desired part of the program.


<h3>Setup.h</h3>
Functions from this file are called from `Source.cpp` at the start of the program. <br>
At the start of the file, I create a classe called M_Vector that just allow me to store and access position and velocity vectors in a way that feels more fluid to me.  

<h2>olcPixelGameEngine.h</h>
See <a href="https://github.com/OneLoneCoder/olcPixelGameEngine">olcPixelGameEngine</a>.

