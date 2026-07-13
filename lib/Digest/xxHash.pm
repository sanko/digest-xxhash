package Digest::xxHash;
use strict;
use warnings;
use base   qw[Exporter];
use Config ();
use XSLoader;

BEGIN {
    our $VERSION = '3.00';
    XSLoader::load __PACKAGE__, $VERSION;
}
our @EXPORT_OK = qw[
    xxhash32 xxhash32_hex
    xxhash64 xxhash64_hex
    xxh3_64 xxh3_64_hex
    xxh3_128 xxh3_128_hex
    xxh3_generate_secret
];
sub xxh3_generate_secret { xxh3_generate_secret_from_seed(@_) }

# Digest-style OO interface
sub new {
    my ( $class, %args ) = @_;
    my $type   = delete $args{type} // 'xxh32';
    my $seed   = delete $args{seed} // 0;
    my $secret = delete $args{secret};
    if (%args) {
        require Carp;
        Carp::croak( 'Unknown arguments: ' . join( ', ', sort keys %args ) );
    }
    my $self = bless { type => $type, seed => $seed, secret => $secret, }, $class;
    $self->_init;
    return $self;
}

sub _init {
    my ($self) = @_;
    my $type = $self->{type};
    if ( $type eq 'xxh32' ) {
        $self->{ctx} = _xxxh32_create();
        _xxxh32_reset( $self->{ctx}, $self->{seed} );
    }
    elsif ( $type eq 'xxh64' ) {
        $self->{ctx} = _xxxh64_create();
        _xxxh64_reset( $self->{ctx}, $self->{seed} );
    }
    elsif ( $type eq 'xxh3_64' ) {
        $self->{ctx} = _xxxh3_create();
        if ( defined $self->{secret} ) {
            _xxxh3_64bits_reset_withSecret( $self->{ctx}, $self->{secret} );
        }
        else {
            _xxxh3_64bits_reset_withSeed( $self->{ctx}, $self->{seed} );
        }
    }
    elsif ( $type eq 'xxh3_128' ) {
        $self->{ctx} = _xxxh3_create();
        if ( defined $self->{secret} ) {
            _xxxh3_128bits_reset_withSecret( $self->{ctx}, $self->{secret} );
        }
        else {
            _xxxh3_128bits_reset_withSeed( $self->{ctx}, $self->{seed} );
        }
    }
    else {
        require Carp;
        Carp::croak( 'Unknown hash type: ' . $type );
    }
}

sub add {
    my ( $self, @etc ) = @_;
    my $type = $self->{type};
    for (@etc) {
        if ( $type eq 'xxh32' ) {
            _xxxh32_update( $self->{ctx}, $_ );
        }
        elsif ( $type eq 'xxh64' ) {
            _xxxh64_update( $self->{ctx}, $_ );
        }
        elsif ( $type eq 'xxh3_64' ) {
            _xxxh3_64bits_update( $self->{ctx}, $_ );
        }
        elsif ( $type eq 'xxh3_128' ) {
            _xxxh3_128bits_update( $self->{ctx}, $_ );
        }
    }
    return $self;
}

sub digest {
    my ($self) = @_;
    my $type = $self->{type};
    return _xxxh32_digest( $self->{ctx} )       if $type eq 'xxh32';
    return _xxxh64_digest( $self->{ctx} )       if $type eq 'xxh64';
    return _xxxh3_64bits_digest( $self->{ctx} ) if $type eq 'xxh3_64';
    return pack 'QQ', _xxxh3_128bits_digest( $self->{ctx} ) if $type eq 'xxh3_128';
}

sub hexdigest {
    my ($self) = @_;
    my $type = $self->{type};
    return _xxxh32_hex( $self->{ctx} )        if $type eq 'xxh32';
    return _xxxh64_hex( $self->{ctx} )        if $type eq 'xxh64';
    return _xxxh3_64bits_hex( $self->{ctx} )  if $type eq 'xxh3_64';
    return _xxxh3_128bits_hex( $self->{ctx} ) if $type eq 'xxh3_128';
}

sub b64digest {
    require MIME::Base64;
    return MIME::Base64::encode_base64( $_[0]->digest, '' );
}

sub clone {
    my ($self) = @_;
    my $clone  = bless {%$self}, ref($self);
    delete $clone->{ctx};
    $clone->_init;
    my $type = $self->{type};
    if ( $type eq 'xxh32' ) {
        _xxxh32_free( $clone->{ctx} );
        $clone->{ctx} = _xxxh32_create();
        _xxxh32_copy( $clone->{ctx}, $self->{ctx} );
    }
    elsif ( $type eq 'xxh64' ) {
        _xxxh64_free( $clone->{ctx} );
        $clone->{ctx} = _xxxh64_create();
        _xxxh64_copy( $clone->{ctx}, $self->{ctx} );
    }
    elsif ( $type eq 'xxh3_64' || $type eq 'xxh3_128' ) {
        _xxxh3_free( $clone->{ctx} );
        $clone->{ctx} = _xxxh3_create();
        _xxxh3_copy( $clone->{ctx}, $self->{ctx} );
    }
    return $clone;
}

sub reset {
    my ($self) = @_;
    $self->_init;
    return $self;
}

sub DESTROY {
    my ($self) = @_;
    return unless defined $self->{ctx};
    my $type = $self->{type};
    if ( $type eq 'xxh32' ) {
        _xxxh32_free( $self->{ctx} );
    }
    elsif ( $type eq 'xxh64' ) {
        _xxxh64_free( $self->{ctx} );
    }
    elsif ( $type eq 'xxh3_64' || $type eq 'xxh3_128' ) {
        _xxxh3_free( $self->{ctx} );
    }
    delete $self->{ctx};
}
1;
__END__

=encoding utf-8

=head1 NAME

Digest::xxHash - xxHash Implementation For Perl

=head1 SYNOPSIS

    # Functional interface (single-shot)
    use Digest::xxHash qw[
        xxhash32 xxhash32_hex
        xxhash64 xxhash64_hex
        xxh3_64  xxh3_64_hex
        xxh3_128 xxh3_128_hex
        xxh3_generate_secret
    ];

    my $hash   = xxhash32( $data, $seed );
    my $hex    = xxhash32_hex( $data, $seed );
    my $hash64 = xxhash64( $data, $seed );
    my $hex64  = xxhash64_hex( $data, $seed );

    my $xxh3_64  = xxh3_64( $data, $seed );
    my $xxh3_hex = xxh3_64_hex( $data, $seed );

    # 128-bit returns 32-char hex string
    my $xxh3_128_hex = xxh3_128_hex( $data, $seed );

    # Digest-style OO interface (streaming)
    my $ctx = Digest::xxHash->new( type => 'xxh3_64', seed => 0 );
    $ctx->add('one');
    $ctx->add('two');
    say $ctx->hexdigest;

    my $clone = $ctx->clone;
    $ctx->reset;

=head1 DESCRIPTION

xxHash is an extremely fast non-cryptographic hash algorithm that works at RAM speed limits. This module is a wrapper
around the official C xxHash library providing both a functional interface for single-shot hashing and a Digest-style
object-oriented interface for streaming/incremental hashing.

The following hash families are supported:

=over 4

=item B<XXH32> - Classic 32-bit hash (9.7 GB/s)

=item B<XXH64> - Classic 64-bit hash (19.4 GB/s)

=item B<XXH3-64> - Modern 64-bit hash, faster than XXH64 on all platforms

=item B<XXH3-128> - Modern 128-bit hash with extra strength

=back

=head1 FUNCTIONAL INTERFACE

These functions are easy to use but are not suitable for incremental hashing.

=head2 C<xxhash32( $data, $seed )>

Calculates a 32-bit hash and returns it as an unsigned 32-bit integer.

=head2 C<xxhash32_hex( $data, $seed )>

Calculates a 32-bit hash and returns it as an 8-character lowercase hex string.

=head2 C<xxhash64( $data, $seed )>

Calculates a 64-bit hash and returns it as an unsigned 64-bit integer.

=head2 C<xxhash64_hex( $data, $seed )>

Calculates a 64-bit hash and returns it as a 16-character lowercase hex string.

=head2 C<xxh3_64( $data, $seed )>

Calculates a 64-bit XXH3 hash and returns it as an unsigned 64-bit integer. This is faster than C<xxhash64(...)> on all
platforms.

=head2 C<xxh3_64_hex( $data, $seed )>

Calculates a 64-bit XXH3 hash and returns it as a 16-character lowercase hex string.

=head2 C<xxh3_128_hex( $data, $seed )>

Calculates a 128-bit XXH3 hash and returns it as a 32-character lowercase hex string.

=head2 C<xxh3_generate_secret( $seed )>

Generates a 192-byte high-entropy secret from a seed. The returned bytes can be used as the C<secret> parameter in
C<new(...)>. This is equivalent to C<XXH3_generateSecret_fromSeed(...)> in the C API.

=head1 OBJECT-ORIENTED INTERFACE

The OO interface follows the same conventions as L<Digest::MD5> and L<Digest::SHA>, making it easy to swap hash
algorithms with minimal code changes.

=head2 C<new( ... )>

    my $ctx = Digest::xxHash->new( type => 'xxh3_64', seed => 42 );

Creates a new hash context. Valid parameters:

=over 4

=item B<type> (required) - One of: C<xxh32>, C<xxh64>, C<xxh3_64>, C<xxh3_128>

=item B<seed> (optional) - A numeric seed, defaults to C<0>

=item B<secret> (optional) - Raw bytes to use as a custom secret (XXH3 types only)

=back

When C<secret> is provided for XXH3 types, it overrides C<seed>. Generate a proper secret with
C<xxh3_generate_secret(...)>.

=head2 add

    $ctx->add($data);
    $ctx->add($chunk1, $chunk2, $chunk3);

Feeds data into the hash context. Can be called multiple times. Returns the context object for method chaining.

=head2 digest

    my $bytes = $ctx->digest;

Returns the raw hash value. For 128-bit hashes, returns 16 bytes (packed two little-endian uint64s).

=head2 hexdigest

    my $hex = $ctx->hexdigest;

Returns the hash value as a lowercase hex string. Length depends on the hash type: 8 chars for XXH32, 16 for
XXH3-64/XXH64, 32 for XXH3-128.

=head2 b64digest

    my $b64 = $ctx->b64digest;

Returns the hash value as base64-encoded data. Requires L<MIME::Base64>.

=head2 clone

    my $copy = $ctx->clone;

Creates an independent copy of the hash context. Modifying C<$copy> does not affect C<$ctx>.

=head2 reset

    $ctx->reset;

Resets the context to its initial state (same seed/secret as when created). The context can then be reused for a new
hash computation.

=head1 SPEED

This module is the fastest xxHash implementation on CPAN and significantly faster than general-purpose hashes.
Benchmarked on Windows x64 with Perl v5.42 (Strawberry, GCC C<-O3 -msse2 -msse4.2>) against L<Crypt::xxHash> and
standard digest modules. Run C<eg/benchmark.pl> to reproduce.

=head2 XXH3 functional interface (MB/s, higher is better)

                          16 bytes    1 KB     64 KB     1 MB
    ------------------------------------------------------------
    xxh3_64              235.5     9,007    22,136    23,371
    xxh3_128             176.9     7,561    20,191    22,113
    Crypt::xxHash xxh64  120.3     5,951    21,531    23,134
    Digest::MD5           76.2       777       907       913

=over 4

=item B<XXH3-64 beats Crypt::xxHash by up to 96%> at small inputs and matches at large inputs

=item B<XXH3-128 beats Crypt::xxHash by up to 266%> at small inputs (3.6x faster at 16 bytes)

=item B<XXH3-128 is 10x faster than MD5> at 1KB

=back

=head2 Streaming interface (1MB fed in 64KB chunks)

    xxh3_64    18,892 MB/s
    xxh3_128   18,656 MB/s
    xxh64      15,324 MB/s
    xxh32       8,272 MB/s
    MD5           923 MB/s

=over 4

=item B<Streaming xxh3_64 is 11% faster> than Crypt::xxHash streaming (18,892 vs 17,091 MB/s)

=item B<Streaming xxh3_128 is 9% faster> (18,656 vs 17,091 MB/s)

=back

=head2 Hex output (functional, 1KB input)

    xxh3_64_hex           8,324 MB/s
    xxh3_128_hex          7,687 MB/s
    xxhash64_hex          6,527 MB/s
    xxhash32_hex          5,532 MB/s
    Crypt::xxh3_64_hex    3,719 MB/s
    Digest::MD5::md5_hex    748 MB/s

=over 4

=item B<Hex functions use manual nibble writes> instead of C<sprintf>, eliminating format string parsing

=item B<xxh3_64_hex is 2.2x faster> than Crypt::xxHash's equivalent

=item B<xxh3_128_hex is 2.1x faster> than Crypt::xxHash's 128-bit hex

=back

=head1 LICENSE

xxHash is covered by the BSD license.

Digest::xxHash is covered by the Artistic 2 license.

=head1 AUTHOR

Sanko Robinson L<https://github.com/sanko>

xxHash by Yann Collet.

=cut
