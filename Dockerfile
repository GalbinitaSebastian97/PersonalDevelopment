# Dockerfile
FROM ubuntu:24.04

# install build tools (add your cross‑compiler/RTE here as needed)
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential cmake git && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# copy sources
COPY . .    

RUN find . -name "Control_Interface.hpp"

# build everything
RUN mkdir build && cd build && cmake .. && make -j$(nproc)

# default action is to run the proof‑of‑concept executable
CMD ["./build/poc"]

