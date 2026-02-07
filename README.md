# 🚀 Space Shooter Ultimate Edition

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.5+-green.svg)](https://www.sfml-dev.org/)

A feature-rich space shooter game built with C++ and SFML, demonstrating object-oriented programming principles and game development concepts.

<img width="1365" height="716" alt="Screenshot 2026-02-07 131229" src="https://github.com/user-attachments/assets/aa15b198-ef6b-4237-9fd2-7208b6ed97d4" />
)

## 📖 Table of Contents

- [About](#about)
- [Features](#features)
- [Screenshots](#screenshots)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [How to Play](#how-to-play)
- [Game Mechanics](#game-mechanics)
- [Project Structure](#project-structure)
- [OOP Concepts Demonstrated](#oop-concepts-demonstrated)
- [Building from Source](#building-from-source)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

## 🎮 About

Space Shooter Ultimate Edition is a classic arcade-style space shooter game developed as an Object-Oriented Programming (OOP) project. The game features multiple levels, various enemy types, power-ups, a challenging boss battle, and stunning visual effects.

**Version:** 5.0  
**Purpose:** Academic OOP Project

## ✨ Features

- **🎯 Progressive Difficulty**: 2 levels with 2 phases each, culminating in an epic boss battle
- **👾 Diverse Enemies**: 6 different enemy types with unique behaviors:
  - Alpha (Basic)
  - Beta (Weaving)
  - Gamma (Player-tracking)
  - Monster (Charging)
  - Phantom (Phasing)
  - Dragon (Mini-boss)
- **💪 Power-up System**: 8 different power-ups including shields, multi-shot, and screen-clearing nukes
- **🎨 Visual Effects**: Particle systems, explosions, screen shake, and animated sprites
- **🎵 Audio**: Background music and sound effects for immersive gameplay
- **💾 High Score System**: Persistent leaderboard with player names
- **🎮 Smooth Controls**: Responsive keyboard input with multiple control schemes
- **🛡️ Boss Battle**: Multi-phase final boss with dynamic attack patterns

## 📸 Screenshots

### Main Menu
<img width="1365" height="716" alt="Screenshot 2026-02-07 131229" src="https://github.com/user-attachments/assets/1c5d62b7-afc2-4c2a-b37a-b49b4f7211de" />

### Gameplay
<img width="1365" height="683" alt="Screenshot 2026-02-07 131351" src="https://github.com/user-attachments/assets/0f9ceb42-2df8-48ed-8a8a-c13658b4be3a" />

### Boss Battle
<img width="1365" height="710" alt="Screenshot 2026-02-07 131752" src="https://github.com/user-attachments/assets/99ffcccc-15bf-4190-bc3f-aa4e14e36d14" />

### Victory Screen
<img width="1092" height="566" alt="Screenshot 2026-02-07 131811" src="https://github.com/user-attachments/assets/d071ba4c-d5ae-4b09-8e81-f4203dc61c5e" />

## 🔧 Prerequisites

Before you begin, ensure you have the following installed:

- **C++ Compiler**: GCC 7.0+ or Clang 5.0+ with C++17 support
- **SFML Library**: Version 2.5 or higher
  - SFML Graphics
  - SFML Window
  - SFML System
  - SFML Audio
- **CMake**: Version 3.10+ (optional, for building)
- **Git**: For cloning the repository

### Installing SFML

#### Windows (MinGW)
```bash
# Download SFML from https://www.sfml-dev.org/download.php
# Extract and add to your compiler's include/lib paths
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsfml-dev
```

#### macOS (Homebrew)
```bash
brew install sfml
```

## 📥 Installation

### Option 1: Quick Start (Release)

1. Download the latest release from the [Releases](https://github.com/yourusername/space-shooter-ultimate/releases) page
2. Extract the archive
3. Run the executable

### Option 2: Build from Source

1. **Clone the repository**
```bash
git clone https://github.com/yourusername/space-shooter-ultimate.git
cd space-shooter-ultimate
```

2. **Install dependencies** (see [Prerequisites](#prerequisites))

3. **Compile the game**

Using g++:
```bash
g++ -o SpaceShooter src/SpaceShooter.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -std=c++17
```

Using CMake:
```bash
mkdir build
cd build
cmake ..
make
```

4. **Run the game**
```bash
./SpaceShooter
```

## 🎮 How to Play

### Controls

| Key | Action |
|-----|--------|
| **Arrow Keys** / **WASD** | Move spaceship |
| **SPACE** | Fire weapons |
| **P** | Pause game |
| **M** | Toggle sound |
| **ESC** | Return to menu / Exit |

### Objective

1. Survive through 2 levels, each with 2 phases
2. Destroy all enemy ships
3. Collect power-ups to upgrade your ship
4. Defeat the final boss: **EMPEROR DESTRUCTON**
5. Achieve the highest score possible!

## 🎯 Game Mechanics

### Power-ups

| Icon Color | Effect |
|------------|--------|
| 🟡 Yellow | Increase weapon power |
| 🟠 Orange | Faster fire rate |
| 🔵 Blue | Shield protection |
| 🟢 Green | Extra life + heal |
| 🟣 Purple | Multi-shot upgrade |
| 🔴 Red | Temporary invincibility |
| 🟣 Dark Purple | Screen nuke (destroys all enemies) |
| 🔷 Cyan | Slow time |

### Scoring System

- Alpha Enemy: 100-140 points
- Beta Enemy: 150-210 points
- Gamma Enemy: 200-280 points
- Monster Enemy: 300-400 points
- Phantom Enemy: 250-340 points
- Dragon Enemy: 500-700 points
- **Combo Multiplier**: Chain kills for up to 10x multiplier!

### Difficulty Levels

- **Easy**: 0.7x damage multiplier
- **Normal**: 1.0x damage multiplier
- **Hard**: 1.5x damage multiplier


## 🏗️ OOP Concepts Demonstrated

This project showcases fundamental Object-Oriented Programming principles:

### 1. **Encapsulation**
- Private member variables with public accessor methods
- Data hiding in `Spaceship`, `Enemy`, and `Boss` classes

### 2. **Inheritance**
- Base `GameObject` class extended by `Spaceship`, `Enemy`, `Bullet`, `PowerUp`
- Specialized enemy classes (`AlphaEnemy`, `BetaEnemy`, etc.) inherit from `Enemy`

### 3. **Polymorphism**
- Virtual functions (`update()`, `draw()`) overridden in derived classes
- Runtime polymorphism through base class pointers

### 4. **Abstraction**
- Abstract game object interface
- Texture and sound management abstraction

### 5. **Composition**
- `GameState` composes multiple game objects
- Particle system integration

### 6. **Design Patterns**
- **Singleton Pattern**: TextureManager, SoundManager
- **Factory Pattern**: Enemy creation
- **Observer Pattern**: Event handling system

## 🔨 Building from Source

### Using CMake (Recommended)

```bash
# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Compile
cmake --build .

# Run
./SpaceShooter
```

### Manual Compilation

#### Linux/macOS
```bash
g++ -o SpaceShooter src/SpaceShooter.cpp \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
    -std=c++17 -O2
```

#### Windows (MinGW)
```bash
g++ -o SpaceShooter.exe src/SpaceShooter.cpp ^
    -I"C:/SFML/include" -L"C:/SFML/lib" ^
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio ^
    -std=c++17
```

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for more details.

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **SFML Team** - For the amazing multimedia library
- **Inspiration** - Classic arcade shooters like Galaga and Space Invaders

## 📧 Contact


Project Link: [https://github.com/muhammad-shozab/space-shooter-ultimate](https://github.com/muhammad-shozab/space-shooter-ultimate)

---

⭐ If you found this project helpful, please consider giving it a star!

**Developed with ❤️ as an OOP Learning Project**
