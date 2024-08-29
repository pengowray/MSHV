## how to make wasm

```
cd wasm
./build.sh
```

## changes to source code in mshv_adapted:

1. #include paths changed

2.
```c
QRegExp rx("(\\d+)");
if (!callsign.contains(rx))
```

changed to 

```c
if (!callsign.containsDigits())
```




