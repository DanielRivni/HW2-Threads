FROM ubuntu:22.04
WORKDIR  /
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y git gcc python3 python3-pip gdb valgrind time mingw-w64 && \
    apt-get clean all && apt-get autoremove