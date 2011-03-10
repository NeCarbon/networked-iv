#!/bin/bash

g++ *.cpp ../Shared/*.cpp -o ivmpsvr -I. -I../Shared/ -ldl
strip --strip-unneeded ivmpsvr
strip --strip-debug ivmpsvr
