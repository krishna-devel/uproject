#!/bin/bash
function build() {
  rm -rf kdtree/build
  mkdir kdtree/build
  cd kdtree/build
  # ls -alth
  cmake ..
  make
}

build

