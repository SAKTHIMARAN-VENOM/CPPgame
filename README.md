# 🚀 Space Shooter — Week 1
### Topics: SFML Setup · Game Loop · Player Class · Movement · Delta Time

---

## 🎯 What Was Built This Week

This is the **foundation** of the Space Shooter game. No bullets or enemies yet — just the core architecture every game needs before anything else can be added.

| Feature | Status |
|---------|--------|
| SFML window (800×600) | ✅ Done |
| Clean game loop | ✅ Done |
| Delta time (frame-rate independent movement) | ✅ Done |
| Player class with spaceship shape | ✅ Done |
| Left / Right arrow key movement | ✅ Done |
| Screen boundary clamping | ✅ Done |
| Scrolling star background | ✅ Done |
| Live FPS counter | ✅ Done |

---

## 🗂️ File Structure

```
Week1/
├── main.cpp       ← Full source code
└── README.md      ← This file
```

> After compiling, place `arial.ttf` in the same folder as the executable for text to display correctly.

---

## 🔧 How to Compile

### Linux / macOS
```bash
# Install SFML (Ubuntu/Debian)
sudo apt-get install libsfml-dev

# Install SFML (macOS)
brew install sfml

# Compile
g++ main.cpp -o SpaceShooter -lsfml-graphics -lsfml-window -lsfml-system -std=c++17

# Run
./SpaceShooter
```

### Windows (MinGW)
```bash
g++ main.cpp -o SpaceShooter.exe -IC:\SFML\include -LC:\SFML\lib ^
    -lsfml-graphics -lsfml-window -lsfml-system -std=c++17
```

### Windows (Visual Studio)
1. Create an Empty C++ Project
2. Add `main.cpp` to Source Files
3. Project → Properties → C/C++ → Additional Include Dirs → `C:\SFML\include`
4. Project → Properties → Linker → Input → Additional Dependencies:
   - `sfml-graphics.lib;sfml-window.lib;sfml-system.lib`
5. Copy all `.dll` files from `C:\SFML\bin` next to the `.exe`

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| ← Left Arrow | Move player left |
| → Right Arrow | Move player right |
| ESC | Quit game |

---

## 🧠 Key Concepts Learned This Week

### 1. The Game Loop
Every game runs the same three-step cycle every frame:
```
while (window is open):
    1. processEvents()  ← read keyboard, mouse, window close
    2. update()         ← move objects, run logic
    3. render()         ← draw everything to the screen
```

### 2. Delta Time
Without delta time, your game runs at different speeds on different computers.

```cpp
float dt = clock.restart().asSeconds();
position.x += speed * dt;  // moves at 300 pixels/second regardless of FPS
```

If the frame takes 0.016s (60 FPS):   move = 300 × 0.016 = **4.8 px**
If the frame takes 0.033s (30 FPS):   move = 300 × 0.033 = **9.9 px**

Both result in the same distance over 1 second. ✅

### 3. OOP — The Player Class

The `Player` class encapsulates everything about the spaceship:

```
Player
├── Data (private)
│   ├── shipShape    ← visual triangle hull
│   ├── wing1/wing2  ← wing shapes
│   ├── engine       ← engine glow
│   ├── speed        ← movement speed
│   └── position     ← current center position
│
└── Methods (public)
    ├── Player(x, y)          ← constructor, set up shapes
    ├── update(dt, window)    ← handle input, move
    ├── render(window)        ← draw to screen
    ├── getPosition()         ← read-only position getter
    └── getBounds()           ← bounding box for collision (used in later weeks)
```

### 4. Boundary Clamping
Keeps the player inside the screen at all times:
```cpp
if (position.x < 22.f)          position.x = 22.f;      // left wall
if (position.x > winW - 22.f)   position.x = winW - 22.f; // right wall
```

### 5. Scrolling Stars
Each star has its own random speed. When a star falls off the bottom, it wraps back to the top — creating an infinite scroll effect with no extra memory allocation.

---

## 🔍 Code Walkthrough

```
main()
  └── Game game;          creates window, player, stars
      └── game.run()
            └── while(window.isOpen())
                  ├── dt = clock.restart().asSeconds()   ← measure frame time
                  ├── processEvents()                    ← check ESC / close
                  ├── update(dt)                         ← move stars + player
                  └── render()                           ← draw everything
```

---

## ⏭️ What's Coming in Week 2

- **Bullet class** — fire bullets with Spacebar
- **Shooting cooldown** — can't spam fire every frame
- **Bullet movement** — travels upward and disappears off-screen
- **std::vector** — store multiple bullets dynamically

---

## 📝 Notes

- The `struct Star` is defined inside the `Game` class — it is only used by the Game so it makes sense to keep it local.
- `ConvexShape` is used for the player hull because it allows any convex polygon, not just rectangles or circles.
- `setFramerateLimit(60)` caps the loop to 60 FPS so the CPU isn't running at 100% unnecessarily.
