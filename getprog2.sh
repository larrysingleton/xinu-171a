#!/bin/bash

cd ~

XINU_HOME=~/xinu/xinu-171a
PROG2_HOME=~/csci8530-171-prog2

rm -rf $PROG2_HOME
mkdir $PROG2_HOME
cd $PROG2_HOME

mkdir -p config
mkdir -p device
mkdir -p include
mkdir -p shell

cp -p $XINU_HOME/config/Configuration config
cp -rp $XINU_HOME/device/led device
cp -p $XINU_HOME/include/led.h include
cp -p $XINU_HOME/include/prototypes.h include
cp -p $XINU_HOME/include/xinu.h include
cp -p $XINU_HOME/shell/xsh_blinkled.c shell

ls -laR $PROG2_HOME

