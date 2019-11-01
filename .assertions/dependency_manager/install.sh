#!/bin/bash
apt-package/install.sh g++
apt-package/install.sh cmake
apt-package/install.sh make
systemlib/install.sh pthread
git-source/install.sh https://github.com/rockerbacon/stopwatch.git v1-no-tests
