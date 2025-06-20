# competitive-programming

my solution and library of competitive programming

## prerequisites

```bash
sudo apt install ninja-build cmake g++-12 libboost-all-dev libeigen3-dev
```

## build

```bash
cmake -G Ninja -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```
