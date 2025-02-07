# How to build small compiler examples

# on macOS

```
 brew install cmake
 export SDKROOT=$(xcrun --sdk macosx --show-sdk-path)
 cmake -DCMAKE_MAKE_PROGRAM=make 
```

# run on lldb

```
    lldb ./s-llvm
    (lldb) b main
    (lldb) process launch -i s-input.txt
```


