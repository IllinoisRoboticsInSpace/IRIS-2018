# IRIS-2018
Code in progress for the new IRIS 2018 autonomous robot

## Building the examples

```
mkdir build && cd build
cmake ..
make -j
```

## Using the debug flag

* (for C++ developer) in coding
```
#ifdef DEBUG
    print some debugging info
    still printing some debugging info
    save some debugging images
#endif
```

* when compiling
```
cmake -DDEBUG=1 ..
```
