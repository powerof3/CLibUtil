# CLibUtil

A header only library of utility functions spun off from CommonLibSSE

## Features

+ string.hpp
  + FNV-1A compile time string hashing with 32bit/64bit implementation.
  + string split and join functions
  + `to_num` conversion
  + case insensitive string comparison and equality
  + and others

+ numeric.hpp
  + float comparison functions 

+ rng.hpp
  + [Xorshiro-cpp](https://github.com/Reputeless/Xoshiro-cpp) RNG with custom seed support

+ distribution.hpp
  + `get_configs` function to retrieve all files containing custom suffix and extension
  + other utility functions used in `Distributor` style mods (SPID, KID)

+ singleton.hpp
  + interface for base singleton class

  
 + simpleINI.hpp
    + convenient [SimpleINI](https://github.com/brofield/simpleini) value getter and setter