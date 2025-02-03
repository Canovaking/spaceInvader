# Space Invader
Project of Embedded Software for the Internet of Things - Kasim Sinan Yildirim - UniTn

| Group 25        | Mail | Contribution |
|--|--|--|
| Alessio De Col | alessio.decol@studenti.unitn.it | Collisions, Inputs (Buttons, Joystick), Game Logics, Enemy Logic|
| Antonio Amabile | antonio.amabile@studenti.unitn.it | Draft C Code, Game Logic, Player Logic, Enemy Logic|
| Marko Markovic | marko.markovic@studenti.unitn.it | Collisions, Graphics, Game Logic, Projectile Logics, Enemy Logic|

## Table of contents
1. [Project Overview](#project-overview)
2. [Project Layout](#project-layout)
3. [Requirements](#requirements)
    1. [Hardware](#hardware)
    2. [Software](#software)
4. [Installation](#installation)
    1. [Hardware](#hardware-1)
    2. [Software](#software-1)
5. [User Guide](#user-guide)
6. [References](#references)

<a name="project-overview"></a>
## Project Overview  
This project is a **retro arcade-style Space Invaders game** developed on the **MSP432 LaunchPad** using the **TI Educational BoosterPack Sensors**. Players control a spaceship and must defend against waves of invading enemies, combining classic gameplay with embedded systems programming.

### Gameplay  
The game challenges players to maneuver their spaceship and shoot down enemies while avoiding incoming attacks.  

### Game Mechanics  
- **Player Controls:** Move the spaceship and shoot enemies.  
- **Lives:** Players start with **three lives**.  
- **Levels:** The game consists of **three progressively difficult levels**.  
- **Enemies:**  
  - **Level 1:** Enemies have a **2% chance** of firing per frame.  
  - **Level 2:** Fire rate increases to **3% per frame**.  
  - **Level 3:** Fire rate rises to **4% per frame**.  
- **Winning Condition:** If all levels are cleared, the player **wins** and their **final score** is displayed.  
- **Scoring System:** A **live score tracking system** is implemented.  




## Project Layout
```
space_invader
├─── Binaries
├─── Includes
├─── Debug
├─── LcdDriver
├─── targetConfigs
├─── src
│   ├─── enemy.c
│   ├─── enemy.h
│   ├─── graphics.c
│   ├─── graphics.h
│   ├─── main.c
│   ├─── player.c
│   ├─── player.h
│   ├─── projectile.c
│   ├─── projectile.h
│   ├─── startup_msp432p401r_ccs.c
│   ├─── system_msp432p401r.c
│   ├─── msp432p401r.cmd
└─── .gitignore
```



<hr>

* [Video Demo](https://youtu.be/KtH1kI1rhxQ?si=1FkxvvTrHPZ7Lirf)
* [Presentation PDF](https://github.com/Canovaking/spaceInvader/blob/main/PresentazioneFinale.pdf)
* [Presentation PPT](https://github.com/Canovaking/spaceInvader/blob/main/PresentazioneFinale.pptx)

<a name="requirements"></a>
## Requirements
<a name="hardware"></a>
### Hardware
1. MSP432P401R Launchpad
2. MSP432P401R BoosterPack

<a name="software"></a>
### Software
1. Code Composer Studio
2. Driverlib

<a name="installation"></a>
## Installation
<a name="hardware-1"></a>
### Hardware
1. Connect the Boosterpack to the MSP432 Launchpad
2. Connect the MSP432 Launchpad to the computer
<a name="software-1"></a>
### Software
1. Clone the repository
2. Open the project in Code Composer Studio
3. Build the project and burn it to the MSP432
4. Download the [driverlib](https://www.ti.com/tool/MSPDRIVERLIB) and [grlib](https://www.ti.com/tool/MSP-GRLIB) libraries
5. Add the libraries to the linker and loader in the project properties as shown below:
    1. Open CCS and go to Projects > Properties
    2. Go to Build > Arm Compiler > Include Options as shown in the image

        <img src="https://github.com/Canovaking/spaceInvader/blob/main/images/Step1.jpeg">
        
    3. Go to Build > Arm Linker > File Search Path as shown in the image

        <img src="https://github.com/Canovaking/spaceInvader/blob/main/images/Step2.jpeg">


<a name="user-guide"></a>
## User Guide
1. Connect the MSP432 to the computer as described in the [installation section](#installation)
2. [Build](#hardware-1) and run the project
3. Run the project
4. Play, click the button to shoot at enemy spacecraft and the pad to move and dodge enemy's shots
5. Have fun :)

<a name="references"></a>
## References
1. [MSP432P401R Launchpad](https://www.ti.com/tool/MSP-EXP432P401R)
2. [MSP432P401R BoosterPack](https://www.ti.com/tool/BOOSTXL-EDUMKII)
3. [Driverlib](https://www.ti.com/tool/MSPDRIVERLIB)
4. [Grlib](https://www.ti.com/tool/MSP-GRLIB)
5. [Code Composer Studio](http://www.ti.com/tool/CCSTUDIO)
6. [MSP432P401R Launchpad User's Guide](https://www.ti.com/lit/ug/slau356i/slau356i.pdf)
7. [MSP432P401R Launchpad Hardware User's Guide](https://www.ti.com/lit/ug/slau356j/slau356j.pdf)
8. [MSP432P401R Launchpad Software User's Guide](https://www.ti.com/lit/ug/slau356k/slau356k.pdf)
9. [MSP432P401R Launchpad BoosterPack User's Guide](https://www.ti.com/lit/ug/slau356l/slau356l.pdf)
10. [MSP432P401R Launchpad BoosterPack Hardware User's Guide](https://www.ti.com/lit/ug/slau356m/slau356m.pdf)
11. [MSP432P401R Launchpad BoosterPack Software User's Guide](https://www.ti.com/lit/ug/slau356n/slau356n.pdf)
12. [Original Game](https://freeinvaders.org/)
