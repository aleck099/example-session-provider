# EasyRPG Multiplayer Session Provider Example



## Dependencies

Boost.Asio

## Build

```
cmake -Bout
cmake --build out
```

## Usage

Set the environment variable `EP_MODULE_PATH` to that output `libmod.so`(on unix) or `mod.dll`(on windows)

Provide server ip and port in environment variable `EP_MODULE_ARG`, example syntax:

```
123.124.125.126:12345
```

