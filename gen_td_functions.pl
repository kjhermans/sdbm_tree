#!/usr/bin/perl

##
## Copyright 2020 K.J. Hermans (kees.jan.hermans@gmail.com)
## This code is part of simpledbm, an API to a dbm on a finite resource.
##

print "/*
** Copyright 2020 K.J. Hermans (kees.jan.hermans@gmail.com)
** This code is part of simpledbm, an API to a dbm on a finite resource.
*/

#ifndef _TD_FUNCTIONS_H_
#define _TD_FUNCTIONS_H_

#ifdef __cplusplus
extern \"C\" {
#endif

";

my @cfiles;

if (opendir(DIR, ".")) {
  while (my $entry = readdir DIR) {
    if ($entry =~ s/\.c$//) {
      push @cfiles, $entry;
    }
  }
  closedir DIR;
}

foreach my $cfile (sort @cfiles) {
  my $code = `cat $cfile.c`;
  if ($code =~ /((int|void)[ \t\r\n]+$cfile\r?\n[ \t]*\([^\)]*\))/s) {
    print "extern\n$1;\n\n";
  }
}

print "#ifdef __cplusplus
}
#endif

#endif
";

1;
