<h1>Billiards</h1>

This is a project I worked on between June 2020 and April 2021 for my A-Level Computer Science project. It was my first time exploring a low-level language* and I was learning as I went along, so much of the code is suboptimal.
For example, I didn't discover what the intended purpose of header files were until after the project; where I was using a header file for the graphical output and mouse input, I just assumed that you only use one .cpp (or equivelant) file, and include additional functionality from header files.
While I did know that C++ is a more performant language than Python (where the majority of my programming experience was at the time), I didn't know about Garbage Collectors or Memory Safety. This means that the project will inevitably have memory leaks, and I hope to revisit the project some time to find and fix them. 

\* C++ is considered a high-level language as it abstracts much of the detail away from the equivelant in machine code, but as you don't get as much help as you do in Python for example, so you can consider C++ to be a 'lower' level than Python.

https://user-images.githubusercontent.com/97246704/169848423-db1c26c3-f9e2-442b-8d64-24bf6719219f.mp4

<h3>Demos on YouTube:</h3>
- <a href="https://www.youtube.com/watch?v=cdiVpsB5q_E">Pool</a> <br>
- <a href="https://www.youtube.com/watch?v=kJlOyvRQ2Ck">Snooker</a>

<h2>Controls</h2>

In this version of Billiards, you have two aiming stages, free aim, and power mode. <br>
You start in free aim mode, where your aim marker will follow your cursor and indicate where the ball should start accelerating towards.

![image](https://user-images.githubusercontent.com/97246704/169852048-25b1f6b2-a397-40be-b108-91c4e282310f.png)


When the left mouse button is pressed, you enter into power mode. You will see a red and a yellow circle appear around the spot where you clicked. The aim marker will no longer track your cursor’s position, but a newly visible power marker will. The power of your shot is based on the distance between the aim marker and the power marker. <br>
To take the shot, release the left mouse button. <br>
The power marker will not track your cursor outside of the Max Power Radius, and if the left mouse button is released while the power marker is inside the Min Power Radius, the shot will be cancelled.

![image](https://user-images.githubusercontent.com/97246704/169852163-e3e9d9d1-10df-443f-806e-4fa52c5b20e2.png)

