#!/usr/bin/python2.7

from subprocess import call

print "Hello, world!"
lines=call(["ls", "-l"])
print lines
