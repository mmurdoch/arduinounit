# Meta 

This code builds the Compare.h header file which unifies comparisions for
many possible string types on the avr platform:

* String
* char *
* const char *
* char []
* const char []
* const char PROGMEM []

It uses template specialization to get an efficient comparision in each
pairwise possibiilty, with the generic definition using only the less than
operator to create all the possible comparisons.


Typical uses:
```
compareBetween("a","b"); // -1
compareBetween(4,5);     //  1
compareEqual("a","a");   //  true
```



