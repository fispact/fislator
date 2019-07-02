# FISpact transLATOR = FISLATOR

Easy to build via cmake, but does require the following dependencies, provided in the subdirectory 'dependencies':
- CLI11 (header only)
- nlohmann_json (header only)

Please use recursive clone to get the dependencies!
```
git clone --recursive -j8 https://github.com/fispact/fislator
```

Then build via:
```bash
mkdir build
cd build
cmake ..
make
```

To convert to csv do:

```bash
./fislator -f test.json --csv
```

Supports multiple file formats (even at the same time). 
The following does both tab and csv files.
```bash
./fislator -f test.json --csv --tab
```

### About
Translates JSON to other common file formats.

- csv
- tabulated?
- xml?
- hdf5?
- ...


Work ongoing...
