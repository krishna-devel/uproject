# docker build -t kykamath/ubuntu_14_04:kdtree .
# docker run -v $PWD/kdtree/tests/basic_tests/data:/kdtree/data -it kykamath/ubuntu_14_04:kdtree
# docker push kykamath/ubuntu_14_04:kdtree
FROM kykamath/ubuntu_14_04:cpp_setup
MAINTAINER Krishna Y. Kamath <krishna.kamath@gmail.com>

# Install basic software
#RUN sudo apt-get update && sudo apt-get install -y software-properties-common && sudo add-apt-repository ppa:george-edison55/cmake-3.x && sudo apt-get update && sudo apt-get install -y g++ cmake cmake-curses-gui vim && sudo apt-get install -y zlib1g-dev

# Copy project files
RUN mkdir /kdtree
COPY kdtree /kdtree

# Build project
COPY build.sh .
RUN ./build.sh
RUN rm -rf ./build.sh

