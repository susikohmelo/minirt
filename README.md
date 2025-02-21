# MiniRT

Real-time and interactive ray tracer for Hive Helsinki 42 coding school. Rendering is done on the CPU due to the project requirements, but is highly optimized using threads, vector registers, and an arena allocator, just to name a few techniques. See the subject file for detailed description of the project requirements. 

## Build Instructions

### Linux

Install a `gcc` or `clang` and `make` if needed and run

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

You can also create your own scenes by writing your own scene description files with format specified in the subject. As an extension to the scene description format, you can also add textures by adding an optional path to a texture file with `.xpm42` file extension, or you can add a number for roughness value. Anything after `#` is treated as a comment and ignored. See the premade scene description files for examples. 

The scenes are interactive! You can move around pressing **WASD**, **space**, or **shift**, and move the camera by clicking the right mouse button and dragging. You can move objects by clicking the left mouse button and dragging or scrolling. Scrolling will zoom in and out, **3** enables skybox, **1** and **2** controls reflection count,  **Up** and **Down** controls FPS optimization. Double clicking objects opens menu for the given object. Pressing the `o` button on the top left corner opens menu for the environment. 