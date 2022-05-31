
<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/ranaxdev/Cloth-Sim">
    <img src="https://github.com/ranaxdev/Cloth-Sim/blob/main/Res/logo.png?raw=true" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Cloth-Simulator</h3>

  <p align="center">
    A spring-model cloth physics simulator made using C/C++
    <br />
    <br />
    <a href="https://github.com/ranaxdev/Cloth-Sim/issues">Report Bug</a>
  </p>
</p>

<hr>



<!-- ABOUT THE PROJECT -->
## Preview
https://user-images.githubusercontent.com/44033302/147727162-45ded536-d4b2-4c49-ac9c-b393bb58db12.mp4

<br>

As of `30/12/2021`, **Cloth-Sim** features a cloth mesh importer (in the form of a WaveFront obj file), a cloth mesh exporter, a cloth-sphere and cloth-plane collision simulation with options to apply forces such as angular velocity and a cloth free-fall simulation with options to apply forces such as wind direction

### Future Releases

Features that I would like to work towards implementing
- Adding textures/lighting and face triangulation
- Improving the spring-model
- Increasing performance (less load with more springs)
- More simulation scenarios

Feel free to suggest some in the issues tab!


## How 
To create a cloth mesh, you simply need an easy `WaveFront obj` editor so that you can import the `obj` file. The simplest way is to just create a plane in blender and add equal number of cut loops on both the horizontal and vertical. Export as quads.


<img src="https://github.com/ranaxdev/Cloth-Sim/blob/main/Res/img1.png?raw=true" alt="Cloth Mesh" style="width:75%;"/>

The program uses the mass-spring-damper model to model the cloth. In this case, each vertex in the mesh would be considered a mass (particle with a fixed weight). After loading in the cloth mesh, springs are created using adjacent and diagonal connections between the particles as shown in the following diagram.

![Spring Model](https://github.com/ranaxdev/Cloth-Sim/blob/main/Res/img2.jpg?raw=true)

Then to simulate the cloth, you apply the spring force and gravity to all of the created springs (and additional ones depending on the simulation such as friction or wind)


<img src="https://github.com/ranaxdev/Cloth-Sim/blob/main/Res/img3.png?raw=true" alt="Force" style="width:50%;"/>


The parameters that worked for me were
```
mass = 0.5
ks = 10
kd = 5
g = -0.981
```

## Installation
### Linux

Dependencies include `OpenGL 4.50`, `libdl`, `glfw3`, `pthread`, `glm`

```bash
git clone https://github.com/ranaxdev/Masters/
cd Animation_Simulation/Cloth-Sim
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../
make
```

### Windows

I'm currently working on releasing an executable so that you don't have to use `cmake` to build it on windows

## Libraries Used

- [GLAD](https://github.com/Dav1dde/glad)
: OpenGL Loading Library

- [GLM](https://github.com/g-truc/glm)
: OpenGL Mathematics

- [GLFW](https://www.glfw.org/)
: A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input 

- [Dear ImGui](https://github.com/ocornut/imgui)
: Bloat-free Graphical User interface for C++ with minimal dependencies 

<!-- LICENSE -->
## License

Distributed under the Apache 2.0 License. See `LICENSE` for more information.


<!-- CONTACT -->
## Contact

S.R. Qaiser - [@pitu_dev](https://twitter.com/pitu_dev) - sc21srq@leeds.ac.uk

Project Link: [https://github.com/ranaxdev/Cloth-Sim](https://github.com/ranaxdev/Cloth-Sim)
