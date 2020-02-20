#!/usr/bin/perl

use strict;

die "Usage: synth-argsub-repair.pl <arch=x86/x64> <# of sym. bytes to checksum> <1=enable symarrays, 2=enable linfnmemlookup, any-other-value=table treatment> <1=verbose, noverbose otherwise>"
  unless @ARGV == 4;
my($arch, $len, $feature, $verbose) = @ARGV;

my $bin = "TODO: binary-file-name";

# my $unused = 
#     $arch eq "x86" ? 
#     `gcc -DLEN=$len -m32 -static $bin.c -g -o $bin` : 
#     `gcc -DLEN=$len -static $bin.c -g -o $bin`;
my $arg_addr = "0x" . substr(`nm $bin | fgrep " B global_str"`, 0, $arch eq "x64" ? 16 : 8);

my @symbolic_arg = ("-symbolic-string","$arg_addr+$len");

my @trace_opts = ("-trace-iterations", "-trace-tables", "-trace-conditions", "-time-stats");

my @verbose_opts = ("-trace-eip", "-trace-binary-paths-bracketed", "-trace-temps", "-trace-offset-limit");

my @table_limit = ();
if ($feature==1) {
  @table_limit = ("-table-limit", 12, "-tables-as-arrays");
} elsif($feature==2) { 
  @table_limit = ("-table-limit", 12, "-enable-linfnmem");
} else { 
  @table_limit = ("-table-limit", 12);
}

my $fuzz_start_addr = "0x" . substr(`nm $bin | fgrep " T main"`, 0, $arch eq "x64" ? 16 : 8);

my @solver_opts = ("-solver", "smtlib-batch", "-solver-path", "z3"
  #, "-save-solver-files"
  #, "-solver-timeout",5,"-timeout-as-unsat"
    );

my @args = ("TODO: path to /exec_utils/fuzzball", 
	    "-arch", $arch,
	    @solver_opts, "-fuzz-start-addr", $fuzz_start_addr,
	    @table_limit,
	   "-linux-syscalls",
	    @trace_opts,
	    @symbolic_arg, ($verbose==1 ? @verbose_opts : ()), 
	    $bin, "--", $bin);
my @printable;
for my $a (@args) {
    if ($a =~ /[\s|<>]/) {
	push @printable, "'$a'";
    } else {
	push @printable, $a;
    }
}
print "@printable\n";
open(LOG, "-|", @args);
while (<LOG>) {
    if ($_ eq "Match\n" ) {
	# $matches++;
    }
    print "  $_";
}
close LOG;
