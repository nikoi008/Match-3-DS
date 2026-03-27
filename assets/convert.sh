#!/bin/bash

BLOCKSDS="${BLOCKSDS:-/opt/wonderful/thirdparty/blocksds/core}"
GRIT="$BLOCKSDS/tools/grit/grit"


$GRIT sprite2d.png -ftB -fh! -gTFF00FF -gb -gB8 -m!

mv *.pal *.img ../nitrofiles/sprite

$GRIT background.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT endless.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT title_bottom.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT title_top.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

mv *.pal *.img *.map ../nitrofiles/bg
