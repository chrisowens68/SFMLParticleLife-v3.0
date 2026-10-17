# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

C++17 / SFML 3 particle-life simulation (macOS), based on the SFMLTemplate layout. Originally a Windows/Visual Studio SFML 2.5 project; ported to SFML 3 and Homebrew.

## Build & Run

```sh
make                  # builds build/debug/play
make run              # build then run (from repo root; paths like res/config.txt are relative)
make clean            # remove the binary and .dSYM
make distclean        # also remove cached ImGui objects
```

- `src/*.cpp` is compiled in one `g++` invocation each build (`-std=c++17 -Wall -O0 -g`). Subdirectories of `src/` are not picked up.
- `lib/imgui/*.cpp` (Dear ImGui 1.91.9b + imgui-SFML 3.0, vendored) are compiled once to `build/debug/imgui/*.o` at `-O2` and reused.
- Links `-lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL`; `-DGL_SILENCE_DEPRECATION` silences macOS OpenGL deprecation warnings.
- There are no tests or linters.

## SFML setup

- `include/SFML` is a symlink to `/usr/local/opt/sfml/include/SFML`; libs from `/usr/local/opt/sfml/lib` (`SFML_PREFIX` in the Makefile).
- `lib/imgui/imconfig.h` includes `imconfig-SFML.h` so ImVec2/ImVec4 convert to/from SFML types.
- SFML 3 has no `sf::Quads`: each particle is 6 vertices (two triangles) in a `sf::PrimitiveType::Triangles` vertex array (`ParticleSystem::setParticleVertices`).

## Code structure

- `src/main.cpp` — window, ImGui "Universe Control" panel, input (R restart, Space pause, Esc quit), loads `res/config.txt`.
- `src/ParticleSystem.*` — particles, force matrix (`Data`), O(n²) force update with wrap-around edges. Velocities are computed for all particles first, then positions updated. `Particle::position` is authoritative; vertices are rebuilt from it each frame.
- `src/Vec2.*`, `src/RandNum.*` — small vector and RNG helpers (single seeded `std::mt19937`).
- `res/universe.htm` — the original JavaScript particle-life reference.
