kinda complicated  
but very flexible

list_BETA   
- supports up to 255-byte array size (previous version limited to 4-byte), 32bit indexing
- sort and find only support 4-byte arrays.
- byte level array management with low level optimisations (expect to break)  
- small array size (10 - 100 byte): 5 - 10% faster than general methods   
- medium array size (1000 - 10000 byte): 10 - 25% faster  
- large array size (1 - 100 MB): 100 - 1000% faster  
