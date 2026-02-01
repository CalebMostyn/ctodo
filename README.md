# ctodo
todo App Built With HTTP Server and Client

# Architecture
Simple C++ backend HTTP server, built using [cpp-httplib](https://github.com/yhirose/cpp-httplib/tree/master), able to be interfaced with a variety of clients. Currently implemented clients include a Python TUI.

Operates on a single file, with a list of tasks that are either completed or uncompleted. Future feature may include the server managing multiple files to have seperate, concurrent lists.

# Build
Includes dependent libs as git submodules, so pull with `--recurse-submodules`.

Build the server with `make`:
```bash
make all # cleans old build artifacts, builds all from src/
# OR make OR make build
./output/ctodo-server -h
```

The Python client can be built with PyInstaller or just ran as a Python script:
```bash
python3 -m venv venv
# build client
make client
./output/ctodo -h
# OR run from script
source venv/bin/activate
python3 client/python/client.py -h
```
