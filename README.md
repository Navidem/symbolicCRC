# symbolicCRC

Symbolically executing CRC checks using FuzzBALL, angr, and Java Ranger.
Please refer to the paper "It Doesn’t Have to Be So Hard: Efficient Symbolic Reasoning for CRCs" published at BAR2020 for more details.

The symbolic execution engines are avaialble at:

- FuzzBALL:
https://github.com/bitblaze-fuzzball/fuzzball
More specifically, the implementation of table treatment, symbolic arrays, and GF2 treatment are in https://github.com/bitblaze-fuzzball/fuzzball/blob/master/execution/formula_manager.ml

- angr:
https://github.com/angr/angr

- Java Ranger:
https://github.com/vaibhavbsharma/java-ranger


# Directory Contents

- crc32-pm.c
The CRC check that uses branching instead of a table-lookup. This is the same version as was used by Jung et al. in the fuzzification paper. 

- crc32-symarr.c 
The CRC check that uses table lookup and was taken from the EternalPass DARPA CGC challenge from the cb-multios repository.

- MyCRC32.java is the Java translation of the fuzzification CRC32 implementation.

- runJRanger.jpf
Driver script to run Java Ranger

- runFBCRC32.pl
Driver script to run FuzzBALL

- run-angrPM.py
Driver script to run angr

