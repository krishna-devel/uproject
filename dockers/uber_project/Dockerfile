# docker build -t kykamath/ubuntu_14_04:uber_project dockers/uber_project/
# docker push kykamath/ubuntu_14_04:uber_project
FROM kykamath/ubuntu_14_04:uber_setup
MAINTAINER Krishna Y. Kamath <krishna.kamath@gmail.com>

# Install basic software
#RUN sudo apt-get update && sudo apt-get install -y software-properties-common && sudo add-apt-repository ppa:george-edison55/cmake-3.x && sudo apt-get update && sudo apt-get install -y g++ cmake cmake-curses-gui vim

# 
RUN mkdir /kdtree 
COPY ../../kdtree /kdtree