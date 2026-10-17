# SFML Particle Life

A "particle life" simulation built with SFML 3 and Dear ImGui (via imgui-SFML) on macOS. Six coloured particle types attract or repel each other according to a randomly generated force matrix, forming emergent clusters.

## Build & Run

Requires SFML 3 from Homebrew (`brew install sfml`).

```sh
make          # builds build/debug/play
make run      # build then run
make clean    # remove the binary
make distclean  # also remove the cached ImGui object files
```

Run from the repo root so `res/` is found.

## Controls

- **Universe Control** panel: particle count, proximity, force multiplier, particle size and friction. Particle count applies on restart.
- **R** restart, **Space** pause/resume, **Esc** quit.

## Layout

```
src/          application sources (main, ParticleSystem, Vec2, RandNum)
lib/imgui/    vendored Dear ImGui 1.91.9b + imgui-SFML 3.0
include/SFML  symlink to Homebrew SFML headers
res/          config.txt, imgui.ini (panel layout), fonts, universe.htm (JS reference version)
build/debug/  build output
```

`res/config.txt` sets the window size, initial particle count/size and friction factor.
