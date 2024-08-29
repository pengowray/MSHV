## how to make wasm

```
cd wasm
./build.sh
```

## changes to source code in mshv_adapted:

1. #include paths changed

2. avoid requring QT regex
```c
QRegExp rx("(\\d+)");
if (!callsign.contains(rx))
// ...changed to...
if (!containsDigits(callsign))
```

3. the loop after `long long int n58=0;` in `pack_unpack_msg77.cpp`

(change not technically required)

