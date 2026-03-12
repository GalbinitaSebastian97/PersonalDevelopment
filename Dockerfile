# Build everything together using CMake
FROM gcc:latest
RUN apt-get update && apt-get install -y cmake

COPY . /app
WORKDIR /app

# Compile both executables at once
RUN mkdir build && cd build && cmake .. && make

# Use the start script to launch them in the SAME environment
RUN chmod +x start.sh
CMD ["./start.sh"]