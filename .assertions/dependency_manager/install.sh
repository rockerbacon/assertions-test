#!/bin/bash
apt-package/install.sh g++
apt-package/install.sh cmake
apt-package/install.sh make
git/install.sh https://github.com/rockerbacon/stopwatch.git v2.1.4 false "src/objs" "src/objs" ""
