# CalVM
CalVM is a 32-bit virtual machine written in fully portable C, which allows it to
run Callisto programs on platforms that Callisto does not support (that is when Callisto
can compile to CalVM)

## Get
```
git clone https://github.com/callisto-lang/calvm --recursive
```

## Compile
```
make
```

Or

```
cc source/*.c -std=c99 -o calvm
```
