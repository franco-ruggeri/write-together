# Collaborative Text Editor
Project for the course System Programming (Programmazione di Sistema) at Politecnico di Torino. The instructions are in Italian, while the rest is in English.

The project is inspired by [Conclave](https://conclave-team.github.io/conclave-site/).

## How to build and run (UNIX commands)
1. mkdir -p build
2. cd build
3. cmake ..
4. cmake --build .
5. ctest . (optional for testing)
6. cmake --install .
7. cd ../bin
8. run executable
   * server: ./server port
   * client: ./client ip_address port
