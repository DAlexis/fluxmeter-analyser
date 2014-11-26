#!/bin/bash

set -e

function plotSeason 
{
     ./proceed-file ./2014 $1-05022014 efm $2 10 16
     ./proceed-file ./2014 $1-05062014 efm $2 18 20

     ./proceed-file ./2014 $1-06062014 efm $2 15 18
     ./proceed-file ./2014 $1-06092014 efm $2 5 8
     ./proceed-file ./2014 $1-06222014 efm $2 12 15
     ./proceed-file ./2014 $1-06242014 efm $2 11 17
     ./proceed-file ./2014 $1-06252014 efm $2 18 20

     ./proceed-file ./2014 $1-07102014 efm $2 8 13
     ./proceed-file ./2014 $1-07292014 efm $2 10 14

     ./proceed-file ./2014 $1-08022014 efm $2 11 14
     ./proceed-file ./2014 $1-08072014 efm $2 15 19
     ./proceed-file ./2014 $1-08152014 efm $2 14 21
     ./proceed-file ./2014 $1-08302014 efm $2 9 11
}

echo 4 > renorm.txt
plotSeason dataIFM $1

echo 4 > renorm.txt
plotSeason dataIPFAN $1

echo 3 > renorm.txt
plotSeason unn-boltek $1
