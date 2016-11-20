# docker build -t kykamath/ubuntu_14_04:uber_setup dockers/uber_setup/
# docker push kykamath/ubuntu_14_04:uber_setup
FROM ubuntu:14.04
MAINTAINER Krishna Y. Kamath <krishna.kamath@gmail.com>

# Install basic software
RUN sudo apt-get update && sudo apt-get install -y software-properties-common && sudo add-apt-repository ppa:george-edison55/cmake-3.x && sudo apt-get update && sudo apt-get install -y g++ cmake cmake-curses-gui vim

# 
#RUN mkdir /project 
#COPY . /project