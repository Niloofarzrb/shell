awk '{ if (++words[$0] > max) { max = words[$0]; maxW=$0 } } END { print maxW }' $1
