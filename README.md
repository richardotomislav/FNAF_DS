> [!IMPORTANT] 
> DISCLAIMER
> 
> **THIS IS A STUDENT PROJECT MADE AS A WAY TO LEARN THE NDS AND C++ PROGRAMMING. NO COPYRIGHT INFRINGMENT IS INTENDED, ALL RIGHTS BELONG TO SCOTT CAWTHON AND STEEL WOOL STUDIOS.**
>
> *If a legal entity wishes to this code to be removed, i'll take it down immediately.*


---

# Five Night's at Freddy's - Nintendo DS edition

### Powered by devKitPro, a lot of nights, coffee and a ruined Christmas.

>[!NOTE]
>
> A highly experimental, colledge assignement game, remake of Five Night's at Freddy's for the Nintendo DS with devKitPro
>
> Project done as a final college assignment for the subject: VJ1214 - VIDEO GAME CONSOLES AND DEVICES.

This is a very experimental clone of the videogame [Five Night's at Freddy's](https://en.wikipedia.org/wiki/Five_Nights_at_Freddy%27s) for the Nintendo DS. The game was developed using the [devKitPro](https://devkitpro.org/) toolchain and the [libNDS](https://libnds.devkitpro.org/) library.

I've opened the source code of my game so my colleages and other people can learn and improve from this code, as this was done by a very much beginner of C++.


### Game Features

* Touch screen with D-PAD support. Doors and cameras are fully functional.
* Five minutes of gameplay.
* Bonnie and Chica AI is i screens.
* Foxy AI is implmented but kinda buggy.
* Freddy is a vegetable.
* Battery levels and time change in real time.
* Experience *zero* sounds and music.

### Technical Features

* Custom renderer logic to handle a TON of images (nevernesters beware).
* Mostly copied AI from the original game.
* Almost all of the code fully documented.
* A lot of spaghetti code.
* Includes a Makefile ready to be used to compile the game.


## Using this repo and building

1. Clone this repo with Github Desktop or cmd, or use the green button above, everything works.
2. Download and install [devKitPro](https://devkitpro.org/) and a NDS emulator (WinDS Pro works fine, DesMume is good too).
3. devkitPro NEEDS to be added to Path, but should be done automatically.
4. Open the folder on VSCode.
5. Open a Terminal there and:
    * Run `make` to compile the game.
    * Run `make clean` to clean the build and `.nds` and `.elf` files.

To make use of the debug functions of this code, you need the `no$gba debug version` of the emulator. You can find it [here](https://problemkaputt.de/gba.htm).


## Credits and assets used

### Assets

* Original camera images taken from:
  * [https://www.reddit.com/r/technicalFNaF/comments/7i6jss/mega_dump/](https://www.reddit.com/r/technicalFNaF/comments/7i6jss/mega_dump/)
* VSCode devKitPro template (so VSCode properly recognises the libNDS functions).
  * [https://github.com/cuibonobo/nds_vscode_template]
* Grit, to convert the game images to something the NDS can read.
  * [https://www.coranac.com/projects/grit/]
* Maxmod, for the sound *(although I wasn't able to use it)*
  * [https://maxmod.devkitpro.org/]

### Aknowledgements

* *problemkapputt* for the `no$gba` emulator.
* *Scott Cawthon* for the original game.
* The UJI teachers for grading this project an 8.5 / 10.
