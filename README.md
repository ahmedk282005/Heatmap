# Heatmap

This project involves developing a program to generate a bitmap image (BMP) in the style of a heatmap from a given dataset. The dataset contains measurement data (x, y, z coordinates) captured by an underwater robot and stored in a CSV file.
The main goal is to visualize these measurements to help an expert team analyze the height profile of a riverbed.

## Example output

Riverbed measurements from `data/data2.csv`, rendered at 400 × 400 px with both interpolation methods:

| Nearest neighbour (`n`) | Bilinear (`b`) |
|---|---|
| ![Heatmap with nearest-neighbour interpolation](docs/nearest-neighbour.png) | ![Heatmap with bilinear interpolation](docs/bilinear.png) |

The scattered measurements are interpolated onto a regular pixel grid, and each height is mapped to one of 10 colours from deep blue (lowest) to dark red (highest). Nearest neighbour copies the closest measurement (visible steps at the edges); bilinear blends the surrounding measurements for smooth transitions.

## Build and run

Requires a C compiler (C99) and `make`. The code compiles cleanly with `-Wall -Wextra -pedantic`.

```bash
make
./heatmap <datafile> <n|b> <width> <height>
```

| Argument | Meaning |
|---|---|
| `datafile` | CSV file with one measurement per line: `x,y,z` |
| `n` / `b` | interpolation: nearest neighbour or bilinear |
| `width`, `height` | size of the output image in pixels (any size, e.g. `600 400`) |

In the image, x runs from left to right and y from top to bottom.

Example:

```bash
./heatmap data/data2.csv b 400 400   # writes heatmap_b.bmp
```

`make clean` removes the program and generated images.

## Context

Coursework for *Procedural Programming for Computer Science* at TUHH (winter semester 2023/24).
