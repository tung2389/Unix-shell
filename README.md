# DUSH shell
- Unix shell that supports running multiple commands in parallel and redirection.

## Project structure
- ```dush.c```: entry point to run the shell.
- ```common/```: some utility functions that are used accross the program.
- ```services/```: main services for the shell, including :
  - **parser** for parsing and validating input into commands.
  - **executor** for executing commands.
  - **logger** for logging useful information.
- ```tester/```: script for running tests.
- ```tests/```: test cases for the shell.

## Usage
- To run the shell:
```bash
make
./dush # Interactive mode
./dush commands.txt # Batch mode
```
- To run test:
```bash
./test_dush.sh
```