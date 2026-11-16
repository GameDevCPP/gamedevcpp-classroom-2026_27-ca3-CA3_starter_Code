# CA3: Non-Trivial Game

## 🤓 Assignment overview and learning outcomes

The goal of this assignment is to:
1. Develop a prototype of a non-trivial game containing features such as AI, multiplayer modes, networking, or physics.
2. Critically evaluate and implement custom memory managers.
3. Critically evaluate and implement techniques for managing game resources which exceed available memory.

This is the big one. CA3 is worth 60% of the module.

## Theme

**Drone.**

Free to interpret this as you wish. Could be literal (a drone as the player
character, an enemy, a delivery mechanic - think factorio etc.), or more abstract. Use it as
guidance...

## Levels

Your game should have a few levels, or an equivalent sense of progression if
it isn't naturally level-based (difficulty curves, procedurally generated
content, an escalating single space, whatever fits your game). Describe your
approach to this in `AssignmentDocumentation.md`'s "Levels" section either way.

## Notes on Carrying-out this Assignment
* In this module students can carry out this assignment independently or in pairs:
  * From the module descriptor: **Cooperative learning/peer tutoring (i.e. Pair-programming for practicals and team-based approaches for some assignments).**
* Please note that students will be graded independently and interviews (probably online) will be used to help assess students.
* If you are pair programming please inform me as to who you are working with.

## Notes on Grading
`GradingRubric_forClass.xlsx` has the full breakdown of marks, based on the module descriptor's grading advice:
* <40%: Inability to design and implement a game. Inability to develop bespoke tools for managing game assets. Inability to critically assess techniques for managing game resources.
* 40-49%: Ability to design and implement a stand-alone game. Ability to develop bespoke tools for managing game assets. Ability to critically assess techniques for managing game resources.
* 50-59%: All the above, plus correctly choosing appropriate data structures, algorithms and patterns.
* 60-69%: All of the above, plus implementing all required features consistently well.
* 70%+: All previous to an excellent level, plus starting to extend the game and framework with features such as resource caching, goal-based behaviour, etc.

I'd point out again: **correctly choose appropriate data structures, algorithms and patterns.**

## Main Aims
* Design and implement a fully functional game using as much from the course plus additional libraries as required.

### Task Notes
1. Use this Classroom50-generated repo, or your own engine/library built during the semester if you'd rather carry that forward instead.
2. Follow `GradingRubric_forClass.xlsx` for the minimum requirements.
3. Use `AssignmentDocumentation.md` (included, with some filler text) to note your significant code additions as you go, and to write up your game design document. The style of the design doc is up to you; there's a short example (`Spiked Acid Game Design Document.pdf`) included for reference.
4. Use component architecture to add new features to the game.
5. Try to put as much of the game in data files rather than in the game code itself. You might find yourself hardcoding game data at the start. It's better to move it out earlier if you can.
6. Keep committing and pushing as you go. You'll build up a record of your work, and can revert if things go pear-shaped.

## What's in this repo

Included: the engine foundation (`engine/`, `lib_ecm`, `lib_tile_level_loader`,
`lib_maths`) and `SpriteComponent`/`ShapeComponent`/`TextComponent` from
earlier practicals.

Not included: A*. You built it in your Steering and Pathfinding repo;
copy it from there. The rubric baseline is now "Basic A\* implemented",
not "as given".

`src/CA3PlaceholderGameTitle/` is your game. Rename it whenever you like
(the folder and the matching line in `CMakeLists.txt`); leaving the name
as-is is fine too, it's not graded.

Each folder under `src/` is its own executable; `CMakeLists.txt` only
compiles files inside one of those folders. A file dropped straight into
`src/` is silently skipped, no error, so keep new files inside your
game's folder.

`src/TileEditor/` and `src/TileDemo/` are extra subprojects, separate
from your submission: a small ImGui tile editor, and a tiny playable
room (textured tiles, a drone you move with arrow keys, wall collision).
Both show one engine build can back more than one executable. Not
required, not graded. Delete either if you don't want them.

### Dear ImGui (optional)

Wired into the engine loop already. Call any `ImGui::` function
(`#include <imgui.h>`) from your scene's `Update()` and it shows up.
Entirely optional.

### Submodules

Box2D is registered but not pulled by a fresh clone. Run:

```bash
git submodule update --init
```

Not mandatory if your game doesn't need physics.

## Optimisation and Memory Management
Typically you should only optimise your game near the end, especially if it's running slowly.
Please note that only a few "culprits" may be responsible for the slow down, e.g. 90% of the time is spent in 10% of the code type situation.
Your first step should be to profile the code to identify the slowest part of your code and fix that only. See the next section for more on this.
### Optimisation techniques:
Some of the advice below is taken from [Tips for Optimizing C/C++ Code](https://people.cs.clemson.edu/~dhouse/courses/405/papers/optimize.pdf)

* Using `++i` rather than `i++` can be faster/more efficient.
* Accessing data in a more-or-less sequential fashion, as stored in physical memory, can dramatically speed up your code (sometimes by an order of magnitude, or more)!
* Change to switch statements if you have many if else if else ... statements.
* When modern CPUs load data from main memory into processor cache, they fetch more than a single value. Instead they fetch a block of memory containing the requested data and adjacent data (a cache
line). This means after array[i][j] is in the CPU cache, array[i][j+1] has a good chance of already being
in cache, whereas array[i+1][j] is likely to still be in main memory.
**NB** IF you do optimise, make sure to document it!

### Memory and Benchmarking
There are many ways to satisfy memory management. No matter what method you use, you will need to be able to measure the difference. One method is to use benchmarking, e.g. running your functions a large number of times in a testing routine. Note you need to be consistent.
The gist of it is as follows:
#### A Naive approach:
1. Initialise stuff
    * e.g. set up your (A) inefficient version, or (B) efficient version.
2. Record current clock time
3. Loop a fixed number of times `N`
4. run method (A) or (B)
5. end Loop
6. Record time.
7. calculate difference in times.
8. *NB* save your results.

The above could be supplemented with profiling from your IDE's profiler or whatever tool supports profiling and memory usage.

#### Using a benchmarking library or code from elsewhere
An example taken from the GitHub repository associated with the book *Game Programming Patterns* is in `benchmarkExample/`. It does something very similar to what's outlined above.
It consists of a header: `utils.h` and test code: `componentTest.cpp`.
`utils.h` contains handy functions such as start/stop profile and some other stuff that you don't need to bother with.
What it does is implement fake game loops with fake components. It answers questions, such as: Which is slower,
```cpp
for (int i = 0; i < NUM_ACTORS; i++)
{
  for (int j = 0; j < NUM_COMPONENTS; j++)
  {
    sum += actors[i].components[j].update();
```
or
```cpp
for (int i = 0; i < NUM_COMPONENTS; i++)
{
  for (int j = 0; j < NUM_ACTORS; j++)
  {
    sum += actors[j].components[i].update();
```

#### Namespaces:
You could use namespaces throughout your project, thus allowing you to use the same name for both optimised and non-optimised methods.
