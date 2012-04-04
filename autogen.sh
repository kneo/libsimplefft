#!/bin/sh
mkdir -f config
mkdir -f m4


autoreconf --force --install -I config -I m4
