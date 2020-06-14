![Pang](https://github.com/WillyTrek19/PANG/raw/master/Wiki/Concept%20Art/PangTitleCard.png)

## Download the [latest release](https://github.com/WillyTrek19/PANG/releases/download/pre-alpha/Bruhmentium.INC_Pang._v.Pre-Alpha.zip)

## GitHub Link: [Click here!](https://github.com/WillyTrek19/PANG)

Recreation of the 1989 classic from arcades by Bruhmentium INC:
 - Guillem Álava (@WillyTrek19) : Founding Code creator (oh, and the audio guy, i guess)
 - Sergi Colomer (@Lladruc37) : Physics & Collisions Overlord
 - Abraham Díaz (@Theran1) : QA & Debugging Menace
 - Arnau Lucena (@TitoLuce) : Particle & Graphics Master
 
![H](https://github.com/WillyTrek19/PANG/raw/master/Wiki/Header/Photos/The%20Smol%20Bois.jpg)

### Gameplay Video
<iframe width="560" height="315" src="https://www.youtube.com/watch?v=1H2wFkoIIFE">
</iframe>
(If the video doesn't work, you can always click [here](https://www.youtube.com/watch?v=1H2wFkoIIFE))

## Description
Pang (also known as Buster Bros. in the North American release and Pomping World in the Japanese) is a videogame created in 1989 by Mitchell Corporation that consists in two players cooperating in destroying various bubbles bouncing around the screen.

In this project, we extracted its assets to recreate this game using C++ and SDL 2.0 libraries.

## How to play (Controls)
### Main controls
- Keyboard:
  - WASD: Move
  - SPACEBAR: Interact (Start)/Shoot
  - ESC: Leave game
- Gamepad:
  - D-Pad/Left Stick: Move
  - A/X/RT (or X/□/R2) Button: Shoot
  - START: Interact (Start)
  - BACK (SELECT): Leave game
  
### Debug Controls
- @Project Sheet Screen
  - F1: Level Selector
    - 1 - 6: Go to level
- Level 1
  - F1: Show hitboxes
  - F2: Cycle through your weapon arsenal
  - F3: Toggle balloon creation mode
    - Mouse Click: Create balloon
    - 1 - 4: Change balloon type
  - F5: GodMode (unables collisions & disables claw sound (because a God should be comfy))
  - F7: Give the player the sweet release of death :)
  - F8: Returns to project sheet screen
  - F9: Win by cheating not only the game; but yourself.
  - F10: Reduces timer to 2 seconds

## Changelog
### Current version: Pre-Alpha
#### Pre-Alpha
- Added the rest of the levels
- Player now has a weapon arsenal (only with debug commands)
- Created all middle transitions
- Module Tileset prepared to support:
  - Non-Destructible tiles
  - Staircases
- Debug functionalities
  - Level selector
  - PowerUp selector
  - Balloon creator when clicked

#### 0.6
- Module Tileset prepared to support wall tiles
- Game is now adapted to the tileset module, including:
  - Physics
  - Collisions
  - Level One
  - Game Scaling
- Polished some bugs

#### 0.5.5
- Started developing Module:
  - Tilesets
- Fixed a lot of bugs
- Added Gamepad support
- Game now plays on fullscreen only
- Game now has a .exe icon

#### 0.5:
- Added Module:
  - Fonts
- Level 1 and winscreen now have a simple UI
- Created time support
- Created point system
- Created life system
- Player can now lose if time arrives to 0 or when they lose all their lives
- Lives reset at title screen
- Checked for ~~spaggetti~~ faulty code
- Adapted README.md to A2 project delivery

#### 0.4.5:
- Added Module:
  - Transitions
- Added external physics support
- Added project info screen, title screen & win screen
- Created win & lose conditions (except for timer & lives)
- Created main loop
- Balloons now divide when hit up to 4 times.

#### 0.4:
- Added Module:
  - Enemies
- Added enemy support
- Added Balloon enemy (Doesn't divide when popped)
- Fixed minor graphical bugs and improved overall code (We made everything cooler :3)
- Added ~~a fuckton of~~ setters and getters

#### 0.3: 
- Added Module:
  - Collisions
- Added collider support
- Player can now Collide with walls
- Added Godmode (toggle on key F5)

#### 0.2:
- Removed unnecessary libraries and resources
- Added Modules:
  - Particles
- Added animation support
- Added particle handler
- Player has new animations:
  - Idle
  - Walking
  - Shoot
- Player can now shoot the basic claw shot

#### 0.1:
- Added Game folder:
  - Main loop
  - Game/Module loop
- Added Modules:
  - Window
  - Render
  - Textures
  - Audio
  - Input
- Added Math Utils
- Created Level 1 display (with no collisions)
- Created Player display (with no collisions nor animations)

## Bugs
#### Pre-Alpha
- Destructible blocks cannot be destroyed, and are instead treated like umbreakable blocks
- The resulting hook of the power wire doesn't change animation

#### 0.6
- Small pixel offset when colliding against a wall
- Sometimes the hook goes through a balloon without destroying the balloon or the hook itself
- Code polishing is required

#### 0.5.5
- Changed Hitboxes
- Fixed collider flashes on debug mode
- Basic Shot:
  - Claw shot now erases at all times
  - Claw shot still can break two balloons at once
- Player: 
  - Fixed minor issue with not being able to shoot after 2nd run
  - Fixed shooting animation. Now you can shoot faster.
  - Fixed dead bounce bug (player only bounces on walls and floor)
  - You now shoot with Spacebar instead of Q

#### 0.5
- Some hitboxes are not equivalent to the original version
- Fixed major collision bugs
- Calibrated blit positioning & counters
- Checked for memory leaks
- Same bugs as 0.4.5
- Basic shot:
  - When spammed, can create particle errors.
- Player:
  - Sometimes collides with balls when dead

#### 0.4.5
- Basic shot:
  - Sometimes doesn't disappear when multiple balls are approaching it. (Shouldn't be a problem; but it's something to take into account).
- Balloon:
  - Hitbox is still not accurate to the original one (it's a square)
- Win Condition:
  - Counter should be reduced.

#### 0.4:
- Player:
  - Fixed animation bugs
  - Added missing particles
- Balloon:
  - Speed.x works with the current code when it shouldn't, no idea how. 
                                              
                                   -It works, it just works ~ Bethesda

#### 0.3:
- Player:
  - Fixed the multiprojectile bug
  
#### 0.2:
- Player:
  - Can shoot multiple projectiles at the same time
- Particle:
  - Needs calibration and polishing
