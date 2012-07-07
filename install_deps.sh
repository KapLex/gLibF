#! /bin/bash

echo "Downloading gLib2D"
if [ -d gLib2D ]; then
yes | rm -r gLib2D
fi
git clone https://github.com/GeeckoDev/gLib2D.git gLib2D
cp gLib2D/glib2d.c gLib2D/glib2d.h .
yes | rm -r gLib2D


