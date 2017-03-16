#!/usr/bin/perl

use Data::Dumper;

my @gccargs;

while (my $testfile = shift @ARGV) {
  if ($testfile =~ /^-/) {
    push @gccargs, $testfile;
    next;
  }
  my $testtext = `cat $testfile`;
  my $tests = eval $testtext;
  foreach my $test (@{$tests}) {
    print "  [UNIT] $testfile ";
    unittest($testfile, $test);
  }
}

exit 0;

##--- functions -----------------------------------------------------------##

sub unittest {
  my $testfile = shift;
  my $test = shift;
  my $code;
  my $ident = "$testfile"; $ident =~ s/^.*\/([^\/]+)$/$1/;
  if ($test->{skip}) {
    print "- Skipped\n";
    return;
  }
  if ($test->{code}) {
    $code = $test->{code};
  } else {
    if (!$test->{function}) {
      die "Test needs function.";
    }
    if (!defined($test->{result})) {
      die "Test needs result.";
    }
    $code =
      join("\n", @{$test->{includes}}) . "\n" .
      "int main(int argc, char* argv[]) {\n  " .
      join(";\n  ", @{$test->{decls}}) . ";\n" .
      "  err_t r = $test->{function}( " . join(",", @{$test->{args}}) . ");\n" .
      "  if (r.code != $test->{result}) { fprintf(stderr, " .
      "\"unittest result \%d\\n\", r.code); return ~0; } else { return 0; }\n" .
      "}\n";
  }
  open CODE, "> /tmp/test_$$.c";
  print CODE $code;
  close CODE;
  my @localargs;
  my $object = $testfile;
  while ($object =~ /\//) {
    $object =~ s/\/[^\/]*$//;
    my $libs = `ls $object/*.a 2>/dev/null`;
    if ($libs) {
      $libs =~ s/\r?\n$//;
      push @localargs, $libs;
    }
    push @localargs, "-I$object";
  }
  push @localargs, @{$test->{libs}};
  my @args = (
    'gcc',
    '-g',
    "/tmp/test_$$.c",
    '-o',
    "/tmp/test_$$.exe",
    @localargs,
    @gccargs,
    @{$test->{gccargs}},
  );
  my $dontremove;
  my $valgrind = ($test->{valgrind} ? "valgrind " : "");
  if (system(@args)) {
    print STDERR "- Compilation failed:" . join(' ', @args) . "\n";
  } elsif (system("$valgrind/tmp/test_$$.exe >/tmp/$ident.log 2>&1")) {
    print STDERR "- Execution failed.\n";
    $dontremove = 1;
  } else {
    print STDERR "- Success.\n";
  }
  if (!$dontremove) {
    system('rm', '-f', "/tmp/test_$$.c", "/tmp/test_$$.exe");
  }
}

1;
