complicated especially list_BETA  

list    
- only supported 4-bytes per cell (32-bit)  
- default memory management.  
- sort, find supported.  
  
list_BETA   
- up to 128-bytes per cell (1024-bit).  
- byte-level memory management that might break, up to 1000% faster on large data.  
- processes raw byte data, require casting before use.  
- sort, find not supported.  
