#!perl
#
#
#
use warnings;
use strict;

my $url = shift;

my $putty = 'C:\Program Files (x86)\putty\PUTTY.EXE';

my $target;
if ( $url =~ m{ssh://([^ ]+)} ) {
    $target = $1;
    $target =~ s{/+$}{};
} else {
    die "Usage: $0 ssh://[user@]host\n";
}
my @args = ("$putty", "-ssh", "$target");

system(@args) == 0
    or print "system @args failed: $?\n";

exit 0;

