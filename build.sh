#!/bin/bash
function build() {
  rm -rf kdtree/build
  mkdir kdtree/build
  cd kdtree/build
  cmake ..
  make
  ./tests/basic_tests/runTests
}

build

