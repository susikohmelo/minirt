![](https://raw.githubusercontent.com/susikohmelo/minirt_dev/refs/heads/main/readme_files/ball.png)


# MiniRT <img align="left" src="https://github.com/susikohmelo/minirt/blob/main/readme_files/score.png" height="120"/>

Real-time interactive ray tracer for Hive Helsinki 42 coding school - from (very nearly) scratch in C.

Rendering is done on the CPU due to the project requirements, but is highly optimized using threads, vector registers, and an arena allocator, just to name a few techniques. See the [subject file](https://github.com/susikohmelo/minirt_dev/blob/main/readme_files/en.subject.pdf) for detailed description of the project requirements. 


![](https://github.com/susikohmelo/minirt_dev/blob/main/readme_files/redblue.jpg?raw=true)

## Features
#### Lighting
* Phong model ( ambient, diffuse & specular )
* Reflections & reflections of reflections, adjustable amount
* Hard shadows

#### Texture/bump mapping
* Albedo maps
* Normal maps
* Roughness maps
* Cube maps ( skybox )

#### Tools
* In-program editor ( camera movement, moving objects, resizing, color, roughness, rotation etc ... )
* Adjustable first-frame resolution scaling for higher FPS
* Multithreading


![](https://github.com/susikohmelo/minirt_dev/blob/main/readme_files/hall.png?raw=true)

## Build Instructions

### Linux

Install `gcc` or `clang` and `make` if needed and run

```bash
git clone https://github.com/susikohmelo/minirt_dev.git
cd minirt_dev
make dev
```

### Windows

You will need [MSYS2 Software and Building Platform](https://www.msys2.org/). Once MSYS2 is installed with `make` and `mingw-w64-ucrt-x86_64-gcc`, open the MSYS2 UCRT64 terminal and run

```bash
git clone https://github.com/susikohmelo/minirt_dev.git
cd minirt_dev
make dev
```

![](https://github.com/susikohmelo/minirt_dev/blob/main/readme_files/test.png?raw=true)

## Usage Instructions

Once the project is built, you can open up a scene by running

````bash
./miniRT <scene_to_open.rt>
````

There are a few premade scenes in `premade_scenes` directory. These scenes include

- `outside.rt` The iconic mirror ball. Hint: press 3 after opening the scene.
- `space.rt` Earth somewhere in void.
- `test.rt` A collection of objects
- `hall.rt` Hallway. Warning: heavy!

You can also create your own scenes by writing your own scene description files with format specified in [the subject](https://github.com/susikohmelo/minirt_dev/blob/main/readme_files/en.subject.pdf). As an extension to the scene description format, you can also add textures by adding an optional path to a texture file with `.xpm42` file extension, or you can add a number for roughness value. Anything after `#` is treated as a comment and ignored. See the premade scene description files for examples. 

### The scenes are interactive!
#### Movement
* Move around pressing **WASD**, **space**, or **shift**
* Rotate the camera by holding the right mouse button and dragging.
#### Object/scene manipulation
* Move objects by clicking the left mouse button and dragging or scrolling.
* Double clicking objects opens menu for the given object.
* Pressing the `o` button on the top left corner opens the menu for the environment.
#### Rendering options 
* **3** enables skybox, **1** and **2** controls reflection count.
* **Up** and **Down** controls resolution scaling for higher FPS.

![](https://github.com/susikohmelo/minirt_dev/blob/main/readme_files/space.png?raw=true)
