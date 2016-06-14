# PACE 2016

https://pacechallenge.wordpress.com/track-b-feedback-vertex-set/

# Compilation
cd src 

make

# Run

The executable file will be created in folder src.

For run \<path_to_file\>/fsolve < \<path to instance\>.graph. It outputs size of minimal feedback vertexset (I hope, it outputs exactly that.)

# Description

That is an implementation of algorithm by Becker et al. with optimized processing of multiple edges.

Namely, suppose A and B have several edges between them, hence A or B belongs to answer and we can recursively branch on two cases. That 
allows improve complexity to  O(2^l * 4^(k-l)), where l is the size of maximal independent set of multiply edges. (Set of multiply edges, 
which haven't got common vertices.) 

