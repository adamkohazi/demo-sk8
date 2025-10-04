
# 🏎️ Countach 4k Intro

**Countach** is a tiny (just **4 kB**), self-contained Windows executable that generates a real-time 3D animation with music.

I started the project to compile the fragments of knowledge I picked up related to shader coding, generative graphics, size coding, 3D modeling and music production.

I submitted the final executable to the **Experience 2023** demo competition, where it placed **3rd**.

For more details on my motivation and process, check out [file_id.diz](assets/file_id.diz) in the assets folder.
Additional information, including a YouTube video, can be found on **[Pouët](https://www.pouet.net/prod.php?which=95507)**.

## ▶️ Running

For **pre-built** executables, go to the [releases](releases/) folder.
A beefy graphics card may be needed to maintain a stable frame rate at higher resolutions, due to the heavy compression.

Press **ESC** at any time to stop the demo.

## 🛠️ Building & Credits

This project was created using **Visual Studio 2022** and targets the **Windows x86/x64** platform. It relies on several external tools and sources, all of which are included in the repository for convenience:

- **[Demo-Framework-4k](https://madethisthing.com/iq/Demo-Framework-4k)** – 4k Windows executable graphics example by **Inigo Quilez (iq)**, that provided the overall structure.
- **[4klang](https://github.com/hzdgopher/4klang)** – Compact softsynth by **Dominik Ries (gopher) and Paul Kraus (pOWL)** of Alcatraz. The .VST was used to record the music and generate the output files. The result is later assembled together with [4klang.asm](tools/4klang/4klang.asm).
- **[NASM](https://www.nasm.us/)** – x86 assembler, used for building 4klang, included in `tools/NASM/`.
- **[Crinkler](https://github.com/runestubbe/Crinkler)** – Compressing linker by **Aske Simon Christensen (Blueberry) and Rune L. H. Stubbe (Mentor)**, used to reduce the final executable size, located in `tools/crinkler/`.
- **[Shader Minifier](https://github.com/laurentlb/Shader_Minifier)** – GLSL minifier by **Laurent Le Brun (LLB / Ctrl-Alt-Test)**, used to shrink shader files, found in `tools/shader_minifier/`

I'm very grateful to the authors of these tools for creating and sharing them, making this project possible.

The build process is optimized for a small file size and is customized with the [.vcxproj](project/countach_4k.vcxproj) file.

## 🌳 Project Structure

An overview of the main files and folders in this project:

```
├───assets/            # Everything else
│   │   file_id.diz      # Descriptor file, packed with the final release
│   └───music/            # Music composition
│       │   countach.4kp   # 4klang preset
│       │   countach.rpp   # Reaper project
│       ├───instruments/    # 4klang instruments individually
│       └───output/        # 4klang output files after recording
│               4klang.h     # song defines for 4klang
│               4klang.inc   # note data (include file for 4klang.asm)
├───build/             # Build directory
├───project/           # Visual studio project files
├───releases/          # Pre-built executables for various resolutions
├───src/               # Source code
│   │   fp.h             # Compact floats - not used here
│   │   glext.h          # OpenGL extensions
│   │   khrplatform.h    # OpenGL platform abstraction
│   │   main.h           # Resolution
│   │   main_win_deb.cpp # Main code for debug builds
│   │   main_win_rel.cpp # Main code for release builds
│   │   synth.h          # Setup audio playback
│   └───shaders/         # Main shader code (before and after minifier)
└───tools/             # External tools
    ├───4klang/          # 4klang source file
    ├───crinkler/        # Clinker executables
    ├───NASM/            # Assembler executables
    └───shader_minifier/ # Shader minifier executable
```

## 📝 Notes

- Depending on build type (Debug/Release), a different main.cpp is loaded.
  - For release builds `entrypoint()` is used as a Crinkler-compatible entry point.
  - For debug builds the default `WinMain()` is used.
- Visuals are entirely created by running a [single fragment shader](src/shaders/fragmentShader.frag). See it on [Shadertoy](https://www.shadertoy.com/view/DtKcRW)!
- I wrote and recorded the music in Reaper using the 4klang .VST plugin. I also included the [reaper project](assets/music/countach.rpp).
- The output file [4klang.inc](assets/music/output/4klang.inc) needs to be assembled together with [4klang.asm](tools/4klang/4klang.asm) into an object file, that is needed for building. This is done automatically during building the project as a custom build event, using NASM. [4klang.h](assets/music/output/4klang.h) also needs to be included in the project.
- Shader minifier is executed during building as a custom build event, using with the following command:
  - `\tools\shader_minifier\shader_minifier.exe -o ..\src\shaders\fragmentShader.inl ..\src\shaders\fragmentShader.frag`
- Crinkler is used as a linker for release builds only. After some tweeking I ended up using the following settings:
  - `/CRINKLER /HASHTRIES:300 /COMPMODE:SLOW /ORDERTRIES:10000 /UNALIGNCODE /REPORT:..\build\out.html`
- Building a 64-bit executable is probably possible, but I never bothered getting it to work. Every release is built for x86.

---
Created by Adam Kohazi (derangedlines)

Distributed under the [MIT license](LICENSE). Please note that some included tools may be subject to different licenses.
