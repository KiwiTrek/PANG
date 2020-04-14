# Pang

Recreation of the 1989 classic from arcades by Bruhmentium INC:
 - Guillem Álava (@WillyTrek19)
 - Sergi Colomer (@Lladruc37)
 - Arnau Lucena (@Theran1)
 - Abraham Díaz (@TitoLuce)

<iframe width="560" height="315" src="https://www.youtube.com/watch?v=UyhP6uLk9Fg">
  </iframe>

## How to install
 - Download the files
 - Execute .exe (Inside Game Folder)
 - Done!

#### Check for more info at the [wiki](https://github.com/WillyTrek19/PANG/wiki)

## Changelog
### Current version: 0.4

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
