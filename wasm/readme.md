## how to make wasm

```
cd wasm
./build.sh
```

## changes to source code in mshv_adapted:

 - #include paths changed


```c
QRegExp rx("(\\d+)");
if (!callsign.contains(rx))
```

changed to 

```c
if (!callsign.containsDigits())
```

