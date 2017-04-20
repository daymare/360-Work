#!/bin/bash

cd ..

make clean

cp diskBackup testimage

make

./project testimage  <testcase/commands.txt



