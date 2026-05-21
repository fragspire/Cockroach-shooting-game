<H1>Cockroach Shooting Game</H1>
<p>Customized copy of the old keypad mobile game space impact using c and c++ and raylib for customizable background and in game pics</p>


File Directory Structure-
├── .gitignore          <- Prevents tracked deployment of compiled *.exe binaries
├── README.md           <- Project description and technical documentation
├── common.h            <- Global dimensions, layout parameters, and shared types
├── main.cpp            <- Core engine loop, state machine, and collision routines
├── player.h / .cpp     <- Player initialization, processing, and Pro-scaling renderers
├── enemy.h / .cpp      <- Autonomous enemy management, timing arrays, and fire gates
└── images/
    ├── insect.png      <- Standardized 32-bit color-converted player texture
    ├── logo.png        <- Standardized 32-bit color-converted enemy wave texture
    └── background.png  <- Crisp static arcade viewport frame texture

    
# Insect vs Logos (Arcade Shooter)

An arcade-style space shooter game built from scratch using **C++** and **Raylib 6.0**. Inspired by the vintage spirit of classic keypad phone games like Nokia's legendary *Space Impact*, this modular desktop application pits a player-controlled insect against waves of incoming corporate logos.

---

## 🛠️ Project Development Journey & Milestones

The current stable build is the result of systematic debugging, asset rendering optimizations, and game-loop engineering:

1. **Modular Architecture Setup**: Split the codebase cleanly into structured components (`main.cpp`, `player.cpp`, `enemy.cpp`, `common.h`) to handle entity positioning, independent update loops, and laser physics securely.
2. **Asset Pipeline & Decoding Solutions**: 
   - Resolved a critical file-parsing issue where modern generated images or non-standard `GRAY_ALPHA` formatted assets threw `WARNING: IMAGE: Failed to load image data` errors inside Raylib's built-in `stb_image` decoder.
   - Handled Windows security blocks (unblocking web-sourced files) and re-encoded assets via standard uncompressed 24-bit/32-bit pixel data blocks using MS Paint.
3. **Sprite Proportional Scaling**: 
   - Overrode high-resolution image dimensions (e.g., 1201x1552 assets causing giant character hitboxes) by implementing fixed arcade bounding boxes (`70x70` for the player, `50x50` for enemies).
   - Integrated `DrawTexturePro()` to seamlessly scale visual textures down into gameplay-accurate hitboxes on the fly without breaking collision logic.
4. **Environment & Layout Framing**: Added a crisp, static custom background frame (`background.png`) that maintains high performance and centers the core arcade action.
5. **State Management & Dynamic Life Mechanics**:
   - Implemented a 5-lifeline degradation system that triggers whenever an enemy laser collides with the player, or the moment an enemy logo crosses past the player's line of defense.
   - Designed a dedicated `GameState` machine managing the regular running loops, an alpha-blended overlay menu for **GAME OVER**, and a completely responsive, mouse-clickable **RETRY** button that wipes active entities and resets the session flawlessly.
6. **Cloud-Safe Version Control**: Configured global `.gitignore` patterns to bypass local binary compilation artifacts (`*.exe`) and cleanly pushed the raw source timeline to GitHub.

---

## 🕹️ Controls & Mechanics

- **Left/Right Arrow Keys**: Slide your Insect player horizontally across the screen.
- **Spacebar**: Launch high-velocity plasma lasers to neutralize oncoming logo waves.
- **Mouse Left-Click**: Interact with the **RETRY** option on the Game Over screen.
- **Lose Conditions**: Your lifelines chunk down from 5 to 0 if a logo drops past your vertical position or if a red enemy bullet registers a pixel-perfect collision with your hitbox.

---

## 🔨 Compilation and Execution Guide

To compile and launch this modular build on your local environment, ensure you have **GCC / MinGW** installed and Raylib globally map-linked to your local storage.

Execute the following strings directly inside your project directory using a terminal or Command Prompt (`cmd`):