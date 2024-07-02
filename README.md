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




