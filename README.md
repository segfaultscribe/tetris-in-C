# 🕹️ Terminal Tetris in C
⚠ Is actively being built (not even in alpha state)<br><br>
Welcome to **Tetoriz**, a minimalist, classic Tetris clone built from scratch in C using the `ncurses` library. Run it in your terminal, relive the blocky nostalgia, and feel the rush as you desperately try to rotate that long piece into the perfect gap.

## 🎮 Features

- ⬇️ Tetrominos fall every few milliseconds
- ⬅️ ➡️ Move left/right with arrow keys
- ⬇️ Soft drop (accelerated fall)
- Playground with falling blocks and borders
- New piece spawns when the last one settles - <span style="color: #E4004B">working on it</span>
- Press `q` to rage-quit

## What's left

- Rotation 
- Collision with stacked blocks
- Line clearing
- Scoring
- Colored blocks
- Game Over screen

---

## 🛠️ Build

```bash
# Exclusive Linux (Debian/Ubuntu) edition
sudo apt install libncurses5-dev libncursesw5-dev

# Compile
make

# Run
./tetoriz
```

## Built With

- C (the OG)

- [ncurses↗](https://invisible-island.net/ncurses/)
 (for terminal UI)

- Way too many `mvprintw()` calls