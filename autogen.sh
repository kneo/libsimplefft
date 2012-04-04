#!/bin/sh
mkdir config
mkdir m4


autoreconf --force --install -I config -I m4
