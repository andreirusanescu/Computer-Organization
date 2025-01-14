FROM jokeswar/base-ctl

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update -yqq && \
    apt-get install -yqq gcc-multilib nasm bc qemu-user gdb && \
    apt-get install -y build-essential valgrind


