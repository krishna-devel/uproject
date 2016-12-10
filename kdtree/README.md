
This is best viewed here: https://github.com/krishna-devel/uproject/tree/master/kdtree

## Software requirements

There are 2 ways to get the environment for running the project:

1. Use the docker image that was built from Ubuntu's docker image for 14.04 (https://hub.docker.com/_/ubuntu/).
This is the easiest way to get the code running and if docker is already installed I would strongly suggest using 
this option
2. Make sure all the software required in installed and get the code from github (or submitted tar.gz file).

### Option 1: Docker image

  To install docker follow the instructions on https://docs.docker.com/engine/installation/
  The prebuilt docker image with kd-tree project for this project is at: kykamath/ubuntu_14_04:uber_project
  You can get it running the following command:

    
    $ docker run -v <PATH_TO_TEST_DATASETS>:/kdtree/data -it kykamath/ubuntu_14_04:uber_project
    

  Replace `PATH_TO_TEST_DATASETS` with the location of the folder containing the datasets with which you want to
  test the kd-tree code. If you just want to run the code against some of the test datasets I have included then
  running this command is sufficient:

    
    $ docker run -it kykamath/ubuntu_14_04:uber_project
    

  Once you run either of these commands you will land on a terminal prompt similar to this:

    
    root@1e822aecd4b1:/#
    

  The code and datasets to test (if you ran the first docker command) are inside the /kdtree folder. The
  datasets are inside the folder named data

    
    root@1e822aecd4b1:/# ls /kdtree
    CMakeLists.txt  bin  build  cmake-build-debug  data  include  lib  main.cpp  src  tests
    
### Option 2: Manual install
  We require the following libraries to build the project.
  
  1. cmake
  2. Zlib (zlib1g-dev)
  3. Boost (libboost-all-dev) 

  Other than these libraries I also use gtest, tbb and Eigen. But they are packaged in the source code for simplicity.

  On an ubuntu machine you can get everything isntalled by running the following command:

    ```  
    sudo apt-get update && sudo apt-get install -y software-properties-common && sudo add-apt-repository ppa:george-edison55/cmake-3.x && sudo apt-get update && sudo apt-get install -y g++ cmake cmake-curses-gui vim && sudo apt-get install -y zlib1g-dev && sudo apt-get install -y libboost-all-dev
    ```
      
  This is the command I run to setup the ubuntu enviroment in kykamath/ubuntu_14_04:uber_project.  

  Get the code either from github repository or extract it from tar-gz file.

## Build project and run tests
  
  The docker image already contains built binaries. So you can skip the building stage and directly jump to tests if you want. Otherwise follow along.

  1. Build project: Go to `build` folder (or create one under kdtree if not present) and delete all the contents. Then build the project using cmake.

    ```
    root@79e81de5fe76:/kdtree/build# rm -rf *
    root@79e81de5fe76:/kdtree/build# cmake .. && make

    -- The C compiler identification is GNU 4.8.4
    -- The CXX compiler identification is GNU 4.8.4
    -- Check for working C compiler: /usr/bin/cc
    -- Check for working C compiler: /usr/bin/cc -- works
        .
        .
        .

    [ 98%] Building CXX object lib/tbb/CMakeFiles/tbbmalloc_static.dir/src/tbbmalloc/tbbmalloc.cpp.o
    [ 99%] Building CXX object lib/tbb/CMakeFiles/tbbmalloc_static.dir/src/tbbmalloc/frontend.cpp.o
    [100%] Building CXX object lib/tbb/CMakeFiles/tbbmalloc_static.dir/src/tbb/itt_notify.cpp.o
    Linking CXX static library libtbbmalloc_static.a
    [100%] Built target tbbmalloc_static

    root@79e81de5fe76:/kdtree/build#
    ```

  2. For testing I have a bunch of unit and integration tests. The integration tests run the code against some
  datasets I generated to evaluate implementation. Some of these integration tests take few minutes to run so they have been 
  commented out by default. The following command runs the tests after the project has been built.

  ```
  root@79e81de5fe76:/kdtree/build# ./tests/basic_tests/runTests

  Running main() from gtest_main.cc
  [==========] Running 53 tests from 14 test cases.
  [----------] Global test environment set-up.
      .
      .
      .
  [----------] Global test environment tear-down
  [==========] 53 tests from 14 test cases ran. (516 ms total)
  [  PASSED  ] 53 tests.

  root@79e81de5fe76:/kdtree/build#
  ```

  To run the longer integration tests, comment out the following lines at the bottom of `tests/basic_tests/KDTreeHandlerTest.cpp`, rebuild the project and run the tests again.

    ~~~~
    TEST_F(KDTreeHandlerTest, test_test_25d) { runTests("test_25d"); }
    TEST_F(KDTreeHandlerTest, test_test_50K) { runTests("test_50K"); }
    ~~~~

    ```
    root@79e81de5fe76:/kdtree/build# cmake .. && make && ./tests/basic_tests/runTests
    ```

## Running kd-tree

  The binaries for the project are under `/kdtree/bin`. The details can be obtained using `--help` argument.

  ```
  root@0a10892d84d1:/kdtree/bin# ./build_kdtree --help
  Options:
    -i [ --input ] arg                    Dataset used to build kd-tree (csv)
    -m [ --model ] arg                    Model to store built kd-tree
    -a [ --split_axis_algorithm ] arg (=HIGHEST_RANGE_AXIS)
                                          Algorithm used to select axis to split
                                          on. Select one of: [
                                          CYCLE_THROUGH_AXES, HIGHEST_RANGE_AXIS
                                          ]
    -s [ --split_position_algorithm ] arg (=MEDIAN_OF_MEDIAN)
                                          Algorithm used to select split
                                          position. Select one of: [ MEDIAN,
                                          MEDIAN_OF_MEDIAN ]
    -p [ --parallel ] arg (=1)            Build model in parallel
    -t [ --print_execution_time ] arg (=1)
                                          Prints execution time in seconds
    -h [ --help ]                         Produce help message
  ```
               
  ```
  root@0a10892d84d1:/kdtree/bin# ./query_kdtree --help
  Options:
    -i [ --input ] arg     Dataset used to build kd-tree (csv)
    -m [ --model ] arg     Model with built kd-tree
    -q [ --queries ] arg   File containing queries to searh
    -n [ --nn_output ] arg Nearest neighbor output
    -h [ --help ]          Produce help message
  ```

  For an example of how to run these commands lets take a look at the `sample_data.csv` file that was provided in the problem set. It contains 3 dimensions and has 1000 samples. We will build the kd-tree for this sample, using HIGHEST_RANGE_AXIS method to select the next dimension to split and MEDIAN_OF_MEDIAN method to select the split point. We will set `parallel` argument to true, as a result of which will use a multi-threaded approach to build the tree instead of recusion.

  ```
  root@0a10892d84d1:/kdtree# ./bin/build_kdtree -i tests/basic_tests/data/sample_data.csv -m sample_data.kdtree
  Reading samples from: tests/basic_tests/data/sample_data.csv
  Algorithm used to select axis to split: 1
  Algorithm used to select split position: 1
  Building model in parallel: 1
  Building kd-tree...
  Writing model to: sample_data.kdtree
  Model was built in 0.131368 seconds.
  ```

  Now to test the model generated in previous step, I generated the queries file that contains the same points use to build the tree. So the nearest point to each of the queries should be the point itself and the eculidian distance between them should be zero. This is stored in `tests/basic_tests/data/sample_data.queries.csv`. You can search for queries and store the result as shown below.

  ```
  root@0a10892d84d1:/kdtree# ./bin/query_kdtree -i tests/basic_tests/data/sample_data.csv -m sample_data.kdtree -n sample_data.results -q tests/basic_tests/data/sample_data.queries.csv
  Using samples from: tests/basic_tests/data/sample_data.csv
  Loading model file from: sample_data.kdtree
  Loading queries from: tests/basic_tests/data/sample_data.queries.csv
  Building kd-tree...
  Storing nearest neighbors in: sample_data.results
  Searched queries in 0.08498 seconds.
  ```

  The output of the results file is as expected:

  ```
  root@0a10892d84d1:/kdtree# head sample_data.results && tail sample_data.results
  0,0.000000
  1,0.000000
  2,0.000000
       .
       .
       .  
  997,0.000000
  998,0.000000
  999,0.000000
  ```


