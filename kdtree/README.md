
This document
- Software requirements
- Getting the code
- Running tests
- Running the binaries

# Software requirements

There are 2 ways to get the environment for running the project:
1. Use the docker image that was built from Ubuntu's docker image for 14.04 (https://hub.docker.com/_/ubuntu/).
This is the easiest way to get the code running.
2. Make sure all the software required in installed and get the code from github (or submitted tar.gz file).

### Option 1: Docker image

- To install docker follow the instructions on https://docs.docker.com/engine/installation/
- The prebuilt docker image with kd-tree project for this project is at: kykamath/ubuntu_14_04:uber_project
  You can get it running the following command:

    ```
    $ docker run -v <PATH_TO_TEST_DATASETS>:/kdtree/data -it kykamath/ubuntu_14_04:uber_project
    ```

  Replace `PATH_TO_TEST_DATASETS` with the location of the folder containing the datasets with which you want to
  test the kd-tree code. If you just want to run the code against some of the test datasets I have included then
  running this command is sufficient:

    ```
    $ docker run -it kykamath/ubuntu_14_04:uber_project
    ```

  Once you run either of these commands you will land on a terminal prompt similar to this:

    ```
    root@1e822aecd4b1:/#
    ```

  The code and datasets to test (if you ran the first docker command) are inside the /kdtree folder. The
  datasets are inside the folder named data

    ```
    root@1e822aecd4b1:/# ls /kdtree
    CMakeLists.txt  bin  build  cmake-build-debug  data  include  lib  main.cpp  src  tests
    ```





Since the code was meant to be working on Ubuntu 14.04LTS,
make sure the software requirements are met by run the following
command:
sudo apt-get update && sudo apt-get install -y software-properties-common && sudo add-apt-repository ppa:george-edison55/cmake-3.x && sudo apt-get update && sudo apt-get install -y g++ cmake cmake-curses-gui vim && sudo apt-get install -y zlib1g-dev && sudo apt-get install -y libboost-all-dev

