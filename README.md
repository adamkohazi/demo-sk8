
# 🛹 Sk8 8k* Intro

**Sk8** is a tiny (*only **5.1 kB**), self-contained Windows executable that generates a real-time 3D animation with music.

I started this project to expand on my previous demoscene production, [Countach](https://github.com/adamkohazi/demo-countach). During the development of that project, I created a wishlist of features that I felt would have been useful. The design goal of this project was to fulfill these items:
- [x] Playback controls for debug builds (pause, play, and seek).
- [x] Fully automated build process (no manual copy-paste or script execution needed).
- [x] Common source files across all builds, controlled by preprocessor directives, enabling or disabling features.
- [x] A compact keyframe engine that:
  - Makes it easy to define keyframes (ideally with a GUI).
  - Performs interpolation between keyframes, supplying the resulting values to the shader. (Eliminating the need to hardcode movement or events in the shader code.)
  - Automatic reloading of keyframe data during execution for debug builds, allowing the fine-tuning of the animation during runtime.
- [ ] Single button release process (compiling for common resolutions, generating screenshot options, and packing final archive).


I submitted the final executable to the **Experience 2025** demo competition, where it placed 3rd.

## ▶️ Running

For **pre-built** executables, go to the [releases](releases/) folder.
A beefy graphics card may be needed to maintain a stable frame rate at higher resolutions, due to the heavy compression.

## 🌳 Project Structure

An overview of the main files and folders in this project:

```
|   LICENSE                 # License information for this project (excluding components)
|   README.md               # YOU ARE HERE
+---assets                  # Resources that are designed seperately
|   |   file_id.diz           # Descriptor file for release package
|   |   screenshot.png        # Screenshot for release package
|   +---keyframes             # Keyframe data for animation
|   |       keyframes.json      # Data file for the keyrame tool
|   |       keyframes.xlsx      # Output for easier overview
|   |       keyframe_data.h     # Output (included by main application)
|   \---music                 # Music composition
|       |   music.rpp           # Reaper project 
|       |   patch.4kp           # 4klang preset
|       +---instruments         # 4klang instruments individually
|       \---output              # 4klang output files after recording
+---build                   # Build directory
+---project                 # Visual studio project files
+---releases                # Pre-built executables for various resolutions
+---src                     # Source code
|   |   audio.h               # Music playback and control
|   |   glext.h               # OpenGL extensions
|   |   keyframes.h           # Keyframe format and interpolation logic
|   |   keyframe_loader.h     # Logic for loading/reloading the keyframe data during runtime
|   |   khrplatform.h         # OpenGL platform abstraction
|   |   main.cpp              # Main code
|   \---shaders               # Main shader code (before and after minifier)
\---tools                   # External tools
    +---4klang                # 4klang source file
    +---crinkler              # Clinker executables
    +---keyframe_editor       # Custom keyframe editor tool
    |   |   main.py             # Application launcher
    |   |   keyframe.py         # Keyframe and node definition
    |   |   timeline.py         # Timeline definition and import/export
    |   \---components          # Additional components for GUI
    |       +---keyframe          # keyframe GUI element
    |       \---timeline          # timeline GUI element
    +---NASM                  # Assembler executables
    +---nlohmann              # JSON for C++
    \---shader_minifier       # Shader minifier executable
```

## 📝 Notes
Here's a collection of notes and helpful snippets on how this all comes together:
#### 🎨 Visuals
- Visuals are entirely created by running a [single fragment shader](src/shaders/fragmentShader.frag). Check it out on [Shadertoy](https://www.shadertoy.com/view/W3SBDt)!
- Shader minifier is executed during building as a custom build event, with the following command:
  - `\tools\shader_minifier\shader_minifier.exe -o ..\src\shaders\fragmentShader.inl ..\src\shaders\fragmentShader.frag`

#### 🎹 Music
- I wrote and recorded the music in Reaper using the 4klang .VST plugin. I also included the [reaper project](assets/music/music.rpp).
- The output file [4klang.inc](assets/music/output/4klang.inc) needs to be assembled together with [4klang.asm](tools/4klang/4klang.asm) into an object file that is needed for building. This is done automatically during building the project as a custom build event, using NASM. [4klang.h](assets/music/output/4klang.h) also needs to be included in the project.

#### 🚶 Animation
- Keyframe data is included as a [header file](assets/keyframes/keyframe_data.h) for release builds, or loaded from the [.json file](assets/keyframes/keyframes.json) for debug builds.
  - Interpolation is performed by the application, and the results, describing the body and board positions, are passed to the shader.
  - For debug builds, the application automatically checks for changes in the [.json file](assets/keyframes/keyframes.json) and reloads it.
  - To save on file size, keyframe data is stored in the [header file](assets/keyframes/keyframe_data.h) as floats, with the last 16 bits cleared. Interpolation mode is then encoded in the last 4 bits. For example, a value of 0.6 with quadratic interpolation becomes 0.5976563692092896f:
  ```
  [0 1 1 1 1 1 1 0 0 1 1 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0]
   | | | | | | | | | | | | | | | |                         \-+-+-+- interpolation mode (4 bits)
   | | | | | | | | | \-+-+-+-+-+-+--------------------------------- significand (7 bits)
   | \-+-+-+-+-+-+-+----------------------------------------------- exponent (8 bits)
   \--------------------------------------------------------------- sign (1 bit)
  ```
- The keyframe editor GUI application is very rudimentary at the moment, I just made something quick and dirty so I can create the animation.
- The keyframe implementation is a bit messy at the moment. Late in development, I realized I created a lot of size overhead with the original data structure. I quickly hacked it so I can submit my entry for the competition deadline, but I'll rework it in the future.

#### 🧩 Other
- The build process is optimized for a small file size and is customized with the [.vcxproj](project/sk8.vcxproj) file.
- Crinkler is used as a linker for release builds only. After some tweaking I ended up using the following settings:
  - `/CRINKLER /HASHTRIES:300 /COMPMODE:SLOW /ORDERTRIES:10000 /UNALIGNCODE /REPORT:..\build\out.html`

Press **ESC** at any time to stop the demo.

## 🛠️ Building & Credits

This project was created using **Visual Studio 2022** and targets the **Windows x86/x64** platform. It relies on several external tools and sources, all of which are included in the repository for convenience:

- **[Demo-Framework-4k](https://madethisthing.com/iq/Demo-Framework-4k)** – 4k Windows executable graphics example by **Inigo Quilez (iq)** that was used as a basis for the project structure.
- **[4klang](https://github.com/hzdgopher/4klang)** – Compact softsynth by **Dominik Ries (gopher) and Paul Kraus (pOWL)** of Alcatraz. The .VST was used to record the music and generate the output files. The result is later assembled together with [4klang.asm](tools/4klang/4klang.asm).
- **[NASM](https://www.nasm.us/)** – x86 assembler, used for building 4klang, included in `tools/NASM/`.
- **[Crinkler](https://github.com/runestubbe/Crinkler)** – Compressing linker by **Aske Simon Christensen (Blueberry) and Rune L. H. Stubbe (Mentor)**, used to reduce the final executable size, located in `tools/crinkler/`.
- **[Shader Minifier](https://github.com/laurentlb/Shader_Minifier)** – GLSL minifier by **Laurent Le Brun (LLB / Ctrl-Alt-Test)**, used to shrink shader files, found in `tools/shader_minifier/`
- **[JSON for Modern C++](https://github.com/nlohmann/json)** – JSON library by Niels Lohmann, used for loading the keyframe data during runtime, included in `tools/nlohmann/`.

Others:
- **[Skateology](https://www.youtube.com/@ashomsky)** by **Adam Shomsky** was used as inspiration and reference footage.

I'm very grateful to the authors of these works for creating and sharing them, making this project possible.

---
Created by Adam Kohazi (derangedlines)

Distributed under the [MIT license](LICENSE). Please note that some included tools may be subject to different licenses.
