# Flexible concatenation for multiple files


## Organisation

The files currently of interest:
 - concatenate_tables.c

Which relies on an a hash table implementation, which holds counts of sequences.
 - ht_array_DYNAMIC.c
 - ht_array_DYNAMIC.h


## Debugging

The sequence "KNKNENK" is listed as follows in the output file out.dat:
```
KNKNENK,0,0,
```
However, it has a count of 2668 in the file T6_retina_100.counts ...


### Reading from files

The script reads correctly the key and its associated counts ; this is tested for in the addition on lines 86-88 :
```
if(strcmp(key,"KNKNENK") == 0) {
    fprintf(stdout, "\n\tFOUND %s in %s in %d counts\n\n", key, argv[f], countI);
}
```
And results in the following output to /dev/stdout :
```
FOUND KNKNENK in data_test/T6_retina_100.counts in 2668 counts
```


### Writing to output

The script writes correctly the key and its associated counts to the output ; this is tested for in the addition on various lines from 36 to 50. Output to /dev/stdout :
```
WRITING KNKNENK 0 0
```

### Storing in the hash table

*ERROR FOUND!*

To find the error I look at the variant "KNKNENK". Thus, the debugging code is tailored to this specific case.

The `ht_expand()` function was using the `ht_set_entry()` function with default arguments causing it to only write the last count value to the array and filling the rest with zeros. An additional argument to the `ht_set_entry()` function, `int copy` taking the value 0 or 1, tracks if the array of the `struct ht_entry` should copied entirely completed with another count value or initialised.


## New BUG!

There is 506 times (out of 13939 lines) where the count written to the output for the second input file is incorrect: the output count is 30984 as the sequence is absent in the input file.
```
EKIIKEK,29,30984,
IKNIRTT,15,30984,
RTNNAER,8,30984,
NKHDNIK,65,30984,
NRHDNIM,7,30984,
DIEKKRV,68,30984,
```
Verifying for all the above with a grep on the input files shows that the count for the first input file are always correct, as the second are always zero - or absent.
```
grep EKIIKEK input_file_1.counts
```

## *Another* New BUG!
The function set_entry() is rewriting over old entry, it is missing a condition, i.e. the section responsible for creating new entries is *always* executed every time the function is called. Need to create conditions to choose between options: adding counts to an existing entry and creating a new entry.


## TODO
 - [ ] See if the code can be better organised for the different usage cases of the `ht_set_entry()` function.
 - [ ] Add *ALL* the securities for the code, which is quite bare so far
 - [X] Setup a makefile for the project
 - [ ] Build a couple of tests to make sure the script is functional when built from source.