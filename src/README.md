# Malaysia Day: Wau Defender 🪁🇲🇾

A 2D arcade survival game built using **C++20** and **Raylib** for WSL/Linux, created to celebrate Malaysia Day! Control a traditional Wau kite as you dodge storm clouds and gather Malaysian cultural items over the iconic Kuala Lumpur skyline.

---

## 📋 3-Day Development & Curriculum Mapping

This project was built over 3 days, integrating software engineering principles, OOP design, and robust testing foundations derived from a 7-day curriculum:

### **Day 1: Project Setup, Class Invariants & Basic OOP (Curriculum Days 1 & 2)**
* **Repository Architecture:** Set up standard directory structure (`src/`, `include/`, `assets/`) and a clean `Makefile` build system.
* **OOP Testing & Class Invariants:** Implemented unit tests (Arrange, Act, Assert pattern) testing `Player` and `Entity` constructors, movement boundaries, and initial state encapsulation.

### **Day 2: Resource Management, STL Containers & Exception Safety (Curriculum Days 3, 4 & 5)**
* **RAII Resource Management:** Built custom RAII wrappers (`TextureWrapper`, `SoundWrapper`) ensuring GPU textures and audio streams automatically free memory upon destruction, eliminating memory leaks.
* **STL Container Management:** Used `std::vector` for entity object pooling (Stars, Hibiscus, Storm Clouds), validating safe container iteration, insertion, and out-of-bounds safety.
* **Failure Modes & Audio Pipeline:** Handled invalid media assets gracefully with null-checks to prevent segmentation faults during audio decoding or texture loading failures.

### **Day 3: Refactoring, UI Polish & Final Submission (Curriculum Days 6 & 7)**
* **Code Refactoring & Fixtures:** Refactored collision detection and drawing methods into clean, modular functions (`GetBounds()`, `Draw()`).
* **Visual & Hitbox Precision:** Aligned circular gradient aura effects (`DrawCircleGradient`) with centered PNG texture coordinates and adjusted cloud scaling parameters.
* **Repository Compliance:** Configured `.gitignore` to exclude build artifacts (`.o` files, `wau_defender` binary), cleaned up system metadata files (`:Zone.Identifier`), and verified clean build compilation via `make clean && make`.

---

## 🛠️ Prerequisites

Ensure you have the following packages installed on your Linux / WSL environment:

```bash
sudo apt update
sudo apt install build-essential libraylib-dev ffmpeg alsa-utils

## How to Build and Run
```bash
git clone [https://github.com/](https://github.com/)<YOUR_USERNAME>/wau_defender.git
cd wau_defender
make clean && make
./wau_defender