<div align="center">
  <p align="center">
   <h1 align="center" style="color:red;">Open Sonic 1 Recreation</h1>
    <img src="https://github.com/GalaxyShad/Open-Sonic-1-Recreation/assets/52833080/e7c05f44-4519-49ae-abcb-ec7aece90ead" /><br/>
   Open source recreation of original Sonic The Hedgehog 1991 game on C++ programming language.
  </p>
</div>

# Media
![OSIR-GHZ1_SM22](https://github.com/GalaxyShad/Open-Sonic-1-Recreation/assets/52833080/1200f042-2c40-4f74-ae94-83cbd1f56eb3)
![OSIR-SYZ](https://github.com/GalaxyShad/Open-Sonic-1-Recreation/assets/52833080/741d4091-513d-4138-bd67-fc97b7beb0c0)

[Watch full gameplay on YouTube](https://www.youtube.com/watch?v=gWmc5dHh4Es&t=118s&ab_channel=GalaxyShad)

# Controls
* Gameplay
  * __Arrows__ - movement
  * __Z__ - Jump
  * __X__ - Spindash (CD Style)
* System
  * __C__ - On/Off Noclip
  * __F__ - Change window size
  * __R__ - Restart level
  * __Q__ - Go to previous level
  * __E__ - Go to next level

# Check list
* Player ✔️
  * Basic Movement ✔️
  * 360 Movement ✔️
  * Collisions ✔️
  * Basic Actions ✔️
* Sounds ✔️
* Music ✔️
* Background parallax ✔️
* HUD ✔️
* Spikes ✔️
* Springs ✔️
* Bosses ❌
* Enemies 
  * Moto Bug ✔️
  * Chopper ✔️
  * Crabmeat ✔️
  * Buzz Bomber ✔️
  * Newtron ❌
  * Ball Hog ❌
  * Batbrain ❌
  * Bomb ❌
  * Burrobot ❌
  * Roller ❌
  * Spikes ❌
  * Unidasu ❌
* Levels \<Collisions\> \<Gimmicks\>
  * Green Hill ✔️✔️
  * Marble ✔️❌
  * Spring Yard ✔️❌
  * Labyrinth ✔️❌
  * Starlight ✔️❌
  * Scrap Brain ✔️❌

# Sources used
* [Sonic 1 Disassembly](https://github.com/sonicretro/s1disasm)
* [Sonic Physics Guide](http://info.sonicretro.org/Sonic_Physics_Guide)
* [Sonic Hacking Guide](http://info.sonicretro.org/SCHG:Sonic_Community_Hacking_Guide)
* [Spirtes Resource](https://www.spriters-resource.com/)
* [Sound Resource](https://www.sounds-resource.com/)

# Build information
## Common
- You need SFML2 library to build it: https://www.sfml-dev.org/download.php
- link following libraries: -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio
- Put output file and SFML dlls to bin folder
## Compiling args
```
<your compiler> src\*.cpp -I include\ -I <path to SFML2 include folder> -L <path to SFML2 lib folder> -o bin\sonic.exe -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio
```
Example for MinGW / GCC
```
g++ src\*.cpp -I include\ -I lib\x64\SFML-2.5.1\include -L lib\x64\SFML-2.5.1\lib -o bin\sonic.exe -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio
```

