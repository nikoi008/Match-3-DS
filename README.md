# Farm Fable  
Farm fable is a match 3 game for the Nintendo DS written using [BlocksDS](https://blocksds.skylyrac.net/) and [NFlib](https://github.com/knightfox75/nds_nflib). It contains features similar to other match 3 games such as animated block swiping, falling and clearing, as well as quality of life features such as automatic detection for possible moves and file saving all made for this amazing console released in 2004.

# Controls
This game is centered around touch controls, so one has to use the stylus to swipe around for matches.
If you are on an emulator and are wondering why the top half of the game is unresponive, it's because the ds only has a touchscreen on the bottom screen, so make sure to swipe using a cursor or finger if you are using a mobile device on the bottom 8x8 grid!  
You can also make a small gap in between both screens by doing `View > Screen Gap > Narrow/Small/Large Border` for a clearer boundary
# DeSmuME
An unmodified version of DeSmuME 0.9.13 is bundled with the rom. To use it simply open the executable, go to `file > open ROM`, and select Farm Fable.nds.

# Building
As mentioned, this game uses [BlocksDS](https://blocksds.skylyrac.net/)  and [NFlib](https://github.com/knightfox75/nds_nflib), as well as [maxmod](https://maxmod.org/) for the sound library. Otherwise all tools are included so you can build it using WSL for windows. Documentation can be found [here](https://blocksds.skylyrac.net/), which includes a get started page for those who are interested.
>Please note that the crop assets are not included in the assets folder due to the raw files being undistributable outside of the compiled game. The game is compiled with the assets, but they must be bought before compiling. Placeholders are added for those that wish to compile the game from source.


# Credits
Thanks to helm3t's [Green Heart asset pack](https://helm3t.itch.io/green-heart-base-pixel-asset-pack) for making the game look amazing
Thanks to SciGho's [Antiquity Script](https://ninjikin.itch.io/font-antiquity-script) for the font used in the title and win screen.
Thanks to the authors and contributors of BlocksDS, Maxmod, and NFLib for the ability to make such games.


# License
This game is licensed under the [MIT](https://opensource.org/license/mit) license
DeSmuME 0.9.13 is licenced under the [GPL 2.0](https://www.gnu.org/licenses/old-licenses/gpl-2.0.html) license, and it's source code can be found [here](https://github.com/TASEmulators/desmume)
