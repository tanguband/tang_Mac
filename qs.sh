#!/bin/sh

export LANG=ja_JP.eucJP
export ANGBAND_X11_FONT_0="-shinonome-gothic-medium-r-*--16"
export ANGBAND_X11_FONT_1="-shinonome-gothic-medium-r-*--14"
export ANGBAND_X11_FONT_2="-shinonome-gothic-medium-r-*--14"
export ANGBAND_X11_FONT_3="-shinonome-gothic-medium-r-*--14"
export ANGBAND_X11_FONT_4="-shinonome-gothic-medium-r-*--14"

export ANGBAND_X11_AT_X_0=452
export ANGBAND_X11_AT_Y_0=0
export ANGBAND_X11_AT_X_1=65
export ANGBAND_X11_AT_Y_1=0
export ANGBAND_X11_AT_X_2=65
export ANGBAND_X11_AT_Y_2=236
export ANGBAND_X11_AT_X_3=0
export ANGBAND_X11_AT_Y_3=850
export ANGBAND_X11_AT_X_4=65
export ANGBAND_X11_AT_Y_4=580

#export ANGBAND_X11_ROWS_0=73
#export ANGBAND_X11_COLS_0=210

export ANGBAND_X11_ROWS_0=64
export ANGBAND_X11_COLS_0=183

export ANGBAND_X11_ROWS_1=15
export ANGBAND_X11_COLS_1=55
export ANGBAND_X11_ROWS_2=23
export ANGBAND_X11_COLS_2=55
export ANGBAND_X11_ROWS_3=12
export ANGBAND_X11_COLS_3=65
export ANGBAND_X11_ROWS_4=18
export ANGBAND_X11_COLS_4=55

xset fp+ /usr/local/X11R6/lib/X11/fonts/japanese
xset fp rehash

cd ~/github/tanguband
./tanguband -utest -- -n5
