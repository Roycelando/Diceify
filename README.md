# 🎲 Diceify

> Transforms any image into a mosaic made of dice.

![C++](https://img.shields.io/badge/C++-100%25-blue) ![Status](https://img.shields.io/badge/status-complete-green)

---

## About

Diceify takes an input image and converts it into a mosaic where each pixel region is replaced by a dice face that best matches the brightness of that area. The darker the region, the more dots on the die.

---

## Examples

| Input | Diceified |
|---|---|
| ![Smile](SmileHikaru.jpg) | ![Bong Cloud](BongCloudHikaru.jpeg) |

![Frown](FrownHikaru.jpeg)

---

## Getting started

### Prerequisites

- A C++ compiler (MSVC, GCC, or Clang)

### Build & run

```bash
git clone https://github.com/Roycelando/Diceify
cd Diceify
# Compile
g++ Source.cpp -o diceify
# Run
./diceify <input_image>
```

---

## How it works

1. Load the input image
2. Divide it into small regions
3. Compute the average brightness of each region
4. Replace each region with the dice face (1–6) that best matches that brightness
5. Output the resulting mosaic image

---

## Project structure

```
Diceify/
├── Source.cpp           # Main C++ source
├── SmileHikaru.jpg      # Sample input
├── BongCloudHikaru.jpeg # Sample output
├── FrownHikaru.jpeg     # Sample output
└── .gitignore
```

---

*By Roycelando*
