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

my $indexstr = '';

foreach my $cfile (sort @cfiles) {
  my $code = `cat $cfile.c`;
  my $decl;
  if ($code =~ /((int|void)[ \t\r\n]+$cfile\r?\n[ \t]*\([^\)]*\))/s) {
    $decl = $1;
    my $esc = $decl;
#    $esc =~ s/_/\\_/g;
#    $esc =~ s/\n/ /g;
    my $esc2 = $cfile;
    $esc2 =~ s/_/\\_/g;
    print "extern\n$decl;\n\n";
    my $comment;
    if ($code =~ /\/\*\*(.*)\*\//s) {
      $comment = $1;
      $comment =~ s/^ ?\*//mg;
      $comment =~ s/\\//g;
      $comment =~ s/_/\\_/g;
    }
    if (! -f "dir_d/latex/$cfile.tex") {
      if (open FILE, '>', "dir_d/latex/fnc_$cfile.tex") {
        print FILE "
\\newpage
\\subsection{$esc2}
\\subsubsection{Declaration} Function prototype:

\\begin{verbatim}
extern
$esc;
\\end{verbatim}

\\subsubsection{Description}

$comment

\\subsubsection{Parameters}
\\subsubsection{Returns}
\\subsubsection{Called by}
";
        close FILE;
      }
    }
    $indexstr .= "\\input{fnc_$cfile.tex}\n";
  }
}

open FILE, '>', 'dir_d/latex/fnc_index.tex'; print FILE $indexstr; close FILE;

print "#ifdef __cplusplus
}
#endif

#endif
";

1;
