#!/bin/bash
function build() {
  rm -rf kdtree/.idea
  rm -rf kdtree/bin	
  rm -rf kdtree/build
  rm -rf kdtree/cmake-build-debug
  mkdir kdtree/build
  cd kdtree/build
  cmake ..
  make
  ./tests/basic_tests/runTests
}

build

