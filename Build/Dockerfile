FROM amd64/ubuntu:18.04

RUN dpkg --add-architecture i386
RUN apt-get update

RUN apt-get install -y git g++-multilib cmake
RUN apt-get install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev
RUN apt-get install -y libgl1-mesa-dev libglu1-mesa-dev
RUN apt-get install -y libudev-dev libfreetype6-dev libopenal-dev libsndfile1-dev

RUN apt-get install -y libx11-dev:i386 libxrandr-dev:i386 libxinerama-dev:i386 libxcursor-dev:i386
RUN apt-get install -y libgl1-mesa-dev:i386 libglu1-mesa-dev:i386
RUN apt-get install -y libudev-dev:i386 libfreetype6-dev:i386 libopenal-dev:i386 libsndfile1-dev:i386
