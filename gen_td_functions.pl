#!/usr/bin/perl

##
## Copyright 2014 K.J. Hermans (kees@pink-frog.com)
## This code is part of simpledbm, an API to a dbm on a finite resource.
##

print "/*
** Copyright 2014 K.J. Hermans (kees@pink-frog.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _TD_FUNCTIONS_H_
#define _TD_FUNCTIONS_H_

#ifdef __cplusplus
extern \"C\" {
#endif

";

if (opendir(DIR, ".")) {
  while (my $entry = readdir DIR) {
    if ($entry =~ s/\.c$//) {
      my $code = `cat $entry.c`;
      if ($code =~ /((int|void)[ \t\r\n]+$entry\r?\n[ \t]*\([^\)]*\))/s) {
        print "extern\n$1;\n\n";
      }
    }
  }
  closedir DIR;
}

print "#ifdef __cplusplus
}
#endif

#endif";

1;
