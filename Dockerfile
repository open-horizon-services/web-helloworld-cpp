FROM ubuntu:18.04

# Install dependencies for building "restbed"
RUN apt-get update && apt-get install -y g++ git cmake 
RUN apt-get install -y rsync zip openssh-server

# Setup the "restbed" library
RUN git clone --recursive https://github.com/corvusoft/restbed.git
RUN mkdir -p restbed/dependency/openssl
RUN cd restbed/dependency/openssl && ./config && make
RUN mkdir -p restbed/build
RUN cd restbed/build && cmake [-DBUILD_SSL=NO] [-DBUILD_TESTS=NO] ..
RUN cd restbed/build && make install

# Dev tools (can be removed for production)
RUN apt update && apt install -y vim curl jq

# Copy in the source file
COPY ./web-hello.cpp /

# Build the binary
RUN g++ -std=c++14 -I /restbed/distribution/include -L /restbed/distribution/library web-hello.cpp -lrestbed -o /web-hello

WORKDIR /
ENV LD_LIBRARY_PATH /restbed/distribution/library
CMD /web-hello

