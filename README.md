# FISpact transLATOR = FISLATOR

Very early days, so just do:

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
