package Digest::xxHash;
use strict;
use base qw(Exporter);
use Config ();
use XSLoader;
BEGIN {
    our $VERSION = '1.01';
    XSLoader::load __PACKAGE__, $VERSION;
}

our @EXPORT_OK = qw[xxhash xxhash_hex];

sub xxhash_hex{unpack('H8', pack('N', xxhash(@_)))}
sub digest_hex{unpack('H8', pack('N', +shift->digest( )))}
1;
__END__

=head1 NAME

Digest::xxHash - xxHash Implementation For Perl

=head1 SYNOPSIS

    use Digest::xxHash qw[xxhash xxhash_hex];

    my $hash = xxhash( $data, $seed );
    my $hex  = xxhash_hex( $data, $seed );
    
    # OO remix!
    
    my $digest = Digest::xxHash->new( $seed );
    $digest->add( $data );
    $digest->add( $more_data );
    my $xxhash = $digest->digest( );
    my $xxhex  = $digest->digest_hex( );

=head1 DESCRIPTION

This module provides both a functional and an OO interface to xxHash functions.
xxHash is an extremely fast (and likely very weak) algorithm that claims to work
at speeds close to RAM limits.

=head1 FUNCTIONAL INTERFACE

These functions are easy to use but aren't very flexible.

=head2 $h = xxhash( $data, $seed )

Calculates a 32 bit hash.

=head2 $h = xxhash_hex( $data, $seed )

Calculates a 32 bit hash and returns it as a hex string.

=head1 OO INTERFACE

The OO interface is a lot more flexible but less succinct.

=head2 $d = Digest::xxHash->new( $seed )

Returns a new Digest::xxHash object. This is a simple constructor.

=head2 $d->add( $data )

Adds data to the spool that will eventually be hashed. You may call this as many
times as needed.

=head2 $d->reset( )

This will reset the data L<added|/"$d-E<< gt >>add( $data )"> to the object. This is 
great if you want to hash several pieces of data with the same seed.

=head2 $h = $d->digest( )

Calculates a 32 bit hash of the data L<added|/"$d-E<< gt >>add( $data )"> to the object.

=head2 $h = $d->digest_hex( )

Calculates a 32 bit hash of the data L<added|/"$d-E<< gt >>add( $data )"> to the object
and returns it as a hex string.

=head1 SPEED

The strength of xxHash isn't very well documented but it's fast. According to
the xxhash project's website, when run in a single thread on a 32bit Windows 7
box with a 3GHz Core 2 Duo processor, xxhash looks a little like:

    Name            Speed       Q.Score   Author
    xxHash          5.4 GB/s     10
    MumurHash 3a    2.7 GB/s     10       Austin Appleby
    SpookyHash      2.0 GB/s     10       Bob Jenkins
    SBox            1.4 GB/s      9       Bret Mulvey
    Lookup3         1.2 GB/s      9       Bob Jenkins
    CityHash64      1.05 GB/s    10       Pike & Alakuijala
    FNV             0.55 GB/s     5       Fowler, Noll, Vo
    CRC32           0.43 GB/s     9
    MD5-32          0.33 GB/s    10       Ronald L. Rivest
    SHA1-32         0.28 GB/s    10

Q.Score is a measure of "quality" of the hash function. It depends on 
successfully passing 
L<SMHasher test set|http://code.google.com/p/smhasher/wiki/SMHasher>. 10 is a
perfect score. Hash functions with a Q.score E<lt> 5 are not listed in this
table.

=head1 LICENSE

xxxHash is covered by the BSD license.

I don't actually care about the wrapper I've written.

=head1 AUTHOR

Sanko Robinson <sanko@cpan.org>

xxHash by Yann Collet.

=cut