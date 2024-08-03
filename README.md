# Build
```sh
make
```

# Usage
ROWS and COLS must fit in the terminal (in the respective playing mode).
Also, the LC_CTYPE locale MUST be UTF8 for the graphical playing mode.
```sh
./connect4 [--no-ai] [--no-fancy] ROWS COLS
./connect4 6 7 # minimum dimensions
```
--no-ai means players take turns via the keyboard
--no-fancy means don't use ncurses to render the game
