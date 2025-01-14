FROM jokeswar/base-ctl

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update

RUN apt-get install -yqq gcc-multilib gdb curl && \
    apt-get install -y build-essential valgrind

RUN curl https://sh.rustup.rs -sSf | sh -s -- -y

ENV PATH="/root/.cargo/bin:${PATH}"

RUN cargo install hw_checker
