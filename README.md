# Space Invader
Project of Embedded Software for the Internet of Things - Kasim Sinan Yildirim - UniTn

| Group 25        | Mail | Contribution |
|--|--|--|
| Alessio De Col | alessio.decol@studenti.unitn.it | Converting the game on ccs studio, implementation of interrupts, resolution of bug, management of input TODO|
| Antonio Amabile | antonio.amabile@studenti.unitn.it | First draft in c of the game, insertion of comments, TODO |
| Marko Markovic | marko.markovic@studenti.unitn.it | Implementation of collision system, upgrade of the game, final review of the project TODO|

<a name="description"></a>
## Description
Our project is a retro arcade-style Space Invader, this game is developed on the MSP432 Launchpad with the TI Educational BoosterPack Sensors. The game challenges players to control a spaceship and defend against waves of invading enemies.
The game logic is implemented with a structured system that includes player movement, enemy behaviors, and interactive shooting mechanics. The player has three lives and must defeat enemies across three progressively difficult levels. Each level introduces different enemy sprites and an increasing probability of enemy attacks. In Level 1, enemies have a 2% chance of firing per frame, increasing to 3% in Level 2 and 4% in Level 3. If the player successfully clears all levels, they win the game, and their final score is displayed along with a live score tracking system.
This embedded systems project demonstrates the integration of hardware and software, utilizing the MSP432 Launchpad's capabilities to create an interactive and engaging gaming experience.


## Table of contents
1. [Description](#description)
2. [Table of contents](#table-of-contents)
3. [Project Layout](#project-layout)
4. [Requirements](#requirements)
    1. [Hardware](#hardware)
    2. [Software](#software)
5. [Installation](#installation)
    1. [Hardware](#hardware-1)
    2. [Software](#software-1)
6. [User Guide](#user-guide)
7. [Options](#options)
8. [References](#references)


## Project Layout
```
├───.vscode
├───src
│   ├───
│       |───
│       |───
│   ├───images
│   ├───msp432
│   │   ├───
│   │   └───
└───.gitignore
```



<hr>

* [Video Demo](https://www.youtube.com/)
* [Presentation PDF](https://github.com/Canovaking/spaceInvader/)
* [Presentation PPT](https://github.com/Canovaking/spaceInvader/)


## Requirements
### Hardware
1. MSP432P401R Launchpad
2. MSP432P401R BoosterPack

### Software
1. Code Composer Studio
2. Driverlib

## Installation
### Hardware
1. Connect the Boosterpack to the MSP432 Launchpad
2. Connect the MSP432 Launchpad to the computer

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



## User Guide
1. Connect the MSP432 to the computer as described in the [installation section](#installation)
2. [Build](#hardware-1) and run the project
3. Run the project
4. Play, click the button to shoot at enemy spacecraft and the pad to move and dodge enemy's shots
5. Have fun :)


## Options



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
