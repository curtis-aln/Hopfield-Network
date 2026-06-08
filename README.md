# Hopfield Neural Network

> A C++ and SFML implementation of the classic Hopfield Network — a recurrent neural network that acts as associative memory, storing and recalling patterns from noisy or corrupted inputs.

<!-- Replace with your actual demo gif/screenshot -->
![Hopfield Network Demo](media/demo.gif)

---

## Table of Contents

- [About](#about)
- [What is a Hopfield Network?](#what-is-a-hopfield-network)
- [How It Works](#how-it-works)
- [Preview](#preview)
- [Dependencies](#dependencies)
- [Building](#building)
- [Usage](#usage)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [References](#references)

---

## About

**Hopfield Neural Network** is a C++ implementation of the Hopfield network — one of the foundational models in neural network theory, originally described by John Hopfield in 1982. The network is trained on a set of binary patterns using Hebbian learning, and is then able to recover those stored patterns from partial or noisy versions of them.

This project renders the network state and recall process in real time using SFML, making it a useful visual learning tool for understanding associative memory, energy minimisation, and early neural network concepts.

---

## What is a Hopfield Network?

A Hopfield network is a **recurrent neural network** where every neuron is connected to every other neuron (but not itself). It behaves as a form of **content-addressable memory** — given a corrupted or incomplete input, the network iteratively updates its neurons until it converges to the nearest stored pattern.

Key properties:

- All connections are **bidirectional and symmetric** — the weight from neuron *i* to neuron *j* equals the weight from *j* to *i*
- The network has an **energy function** that monotonically decreases with each update, guaranteeing convergence
- Patterns are stored by training the **weight matrix** using the Hebbian learning rule: *"neurons that fire together, wire together"*
- Storage capacity is approximately **0.138 × N** patterns for a network of N neurons before spurious states dominate

---

## How It Works

### Training (Hebbian Learning)

For each pattern to be stored, the weight matrix W is updated as:

```
W[i][j] += p[i] * p[j]   for all i ≠ j
W[i][i]  = 0              (no self-connections)
```

Where `p[i]` is the ±1 value of neuron `i` in the pattern. After training on all patterns, W encodes the entire set of memories.

### Recall (Energy Minimisation)

1. The network is initialised with a **corrupted or partial** version of a stored pattern.
2. Neurons update their states asynchronously: each neuron fires (+1) or suppresses (-1) based on the weighted sum of its neighbours.
3. Each update step decreases (or maintains) the network's **energy**:  
   `E = -½ Σᵢⱼ W[i][j] * s[i] * s[j]`
4. The process repeats until the network reaches a **stable state** (a local energy minimum), which corresponds to the recalled pattern.

### Visualisation

The network state is rendered in real time with SFML — each neuron is displayed as a pixel or cell, showing the current ±1 state. You can watch the network "snap" from a noisy input toward a clean stored pattern over successive update steps.

---

## Preview

<!-- Replace these with your actual media once uploaded to the media/ folder -->

| | |
|---|---|
| ![Stored Pattern](media/stored_pattern.png) | ![Recall from Noise](media/recall.gif) |
| A stored pattern | Recall from a corrupted input |

| ![Energy Convergence](media/energy.png) |
|---|
| Energy decreasing toward a stable minimum |

---

## Dependencies

| Dependency | Version | Notes |
|---|---|---|
| [CMake](https://cmake.org/) | 3.15+ | Build system |
| [SFML](https://www.sfml-dev.org/) | 2.5+ | Rendering & windowing |
| C++ Compiler | C++17 | MSVC (Visual Studio 2019+) recommended on Windows |

---

## Building

### Prerequisites

- [CMake 3.15+](https://cmake.org/download/)
- [SFML 2.5+](https://www.sfml-dev.org/download.php) — installed or locatable via `CMAKE_PREFIX_PATH`
- A C++17-compatible compiler (MSVC recommended on Windows)

### Steps

**1. Clone the repository**

```bash
git clone https://github.com/curtis-aln/Hopfield-Neural-Network.git
cd Hopfield-Neural-Network
```

**2. Create a build directory**

```bash
mkdir build
cd build
```

**3. Configure with CMake**

```bash
cmake ..
```

If CMake cannot locate SFML automatically, specify the path:

```bash
cmake .. -DCMAKE_PREFIX_PATH="C:/path/to/SFML"
```

**4. Build**

```bash
cmake --build . --config Release
```

**5. Run**

```bash
./HopfieldNN        # Linux / macOS
HopfieldNN.exe      # Windows
```

> **Windows note:** Ensure the SFML DLLs (`sfml-graphics-2.dll`, `sfml-window-2.dll`, `sfml-system-2.dll`) are in the same directory as the executable, or that SFML is on your system `PATH`.

---

## Usage

Run the executable — an SFML window will open showing the network in action.

```bash
./HopfieldNN
```

The simulation will:
1. Train the weight matrix on the configured set of stored patterns
2. Initialise the network with a noisy version of one of those patterns
3. Run the recall process in real time, updating neuron states each frame until convergence

Close the window to exit.

---

## Configuration

Parameters are hardcoded in the source files under `src/`. Key values to find and adjust:

| Parameter | Description |
|---|---|
| `NEURON_COUNT` | Total number of neurons (grid width × height) |
| `STORED_PATTERNS` | The set of binary patterns (±1) to train on |
| `NOISE_LEVEL` | Fraction of bits to flip when generating the corrupted input |
| `UPDATE_MODE` | Asynchronous (one neuron per step) or synchronous (all at once) |
| `WINDOW_WIDTH` / `WINDOW_HEIGHT` | Render window resolution |

After changing any parameter, rebuild using the steps in the [Building](#building) section.

---

## Contributing

Contributions are welcome! Whether it's adding new features (e.g. interactive pattern drawing, energy graph overlay, modern Hopfield network variant), fixing a bug, or improving documentation:

1. Fork the repository
2. Create a new branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add your feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

Please include a clear description of what changed and why.

---

## References

- Hopfield, J.J. (1982). [Neural networks and physical systems with emergent collective computational abilities](https://www.pnas.org/doi/10.1073/pnas.79.8.2554). *PNAS*, 79(8), 2554–2558.
- [Hopfield network — Wikipedia](https://en.wikipedia.org/wiki/Hopfield_network)
- [SFML documentation](https://www.sfml-dev.org/documentation/)
