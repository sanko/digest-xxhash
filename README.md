# NAME

Digest::xxHash - xxHash Implementation For Perl

# SYNOPSIS

```perl
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
```

# DESCRIPTION

xxHash is an extremely fast non-cryptographic hash algorithm that works at RAM speed limits. This module is a wrapper
around the official C xxHash library providing both a functional interface for single-shot hashing and a Digest-style
object-oriented interface for streaming/incremental hashing.

The following hash families are supported:

- **XXH32** - Classic 32-bit hash (9.7 GB/s)
- **XXH64** - Classic 64-bit hash (19.4 GB/s)
- **XXH3-64** - Modern 64-bit hash, faster than XXH64 on all platforms
- **XXH3-128** - Modern 128-bit hash with extra strength

# FUNCTIONAL INTERFACE

These functions are easy to use but are not suitable for incremental hashing.

## `xxhash32( $data, $seed )`

Calculates a 32-bit hash and returns it as an unsigned 32-bit integer.

## `xxhash32_hex( $data, $seed )`

Calculates a 32-bit hash and returns it as an 8-character lowercase hex string.

## `xxhash64( $data, $seed )`

Calculates a 64-bit hash and returns it as an unsigned 64-bit integer.

## `xxhash64_hex( $data, $seed )`

Calculates a 64-bit hash and returns it as a 16-character lowercase hex string.

## `xxh3_64( $data, $seed )`

Calculates a 64-bit XXH3 hash and returns it as an unsigned 64-bit integer. This is faster than `xxhash64(...)` on all
platforms.

## `xxh3_64_hex( $data, $seed )`

Calculates a 64-bit XXH3 hash and returns it as a 16-character lowercase hex string.

## `xxh3_128_hex( $data, $seed )`

Calculates a 128-bit XXH3 hash and returns it as a 32-character lowercase hex string.

## `xxh3_generate_secret( $seed )`

Generates a 192-byte high-entropy secret from a seed. The returned bytes can be used as the `secret` parameter in
`new(...)`. This is equivalent to `XXH3_generateSecret_fromSeed(...)` in the C API.

# OBJECT-ORIENTED INTERFACE

The OO interface follows the same conventions as [Digest::MD5](https://metacpan.org/pod/Digest%3A%3AMD5) and [Digest::SHA](https://metacpan.org/pod/Digest%3A%3ASHA), making it easy to swap hash
algorithms with minimal code changes.

## `new( ... )`

```perl
my $ctx = Digest::xxHash->new( type => 'xxh3_64', seed => 42 );
```

Creates a new hash context. Valid parameters:

- **type** (required) - One of: `xxh32`, `xxh64`, `xxh3_64`, `xxh3_128`
- **seed** (optional) - A numeric seed, defaults to `0`
- **secret** (optional) - Raw bytes to use as a custom secret (XXH3 types only)

When `secret` is provided for XXH3 types, it overrides `seed`. Generate a proper secret with
`xxh3_generate_secret(...)`.

## add

```
$ctx->add($data);
$ctx->add($chunk1, $chunk2, $chunk3);
```

Feeds data into the hash context. Can be called multiple times. Returns the context object for method chaining.

## digest

```perl
my $bytes = $ctx->digest;
```

Returns the raw hash value. For 128-bit hashes, returns 16 bytes (packed two little-endian uint64s).

## hexdigest

```perl
my $hex = $ctx->hexdigest;
```

Returns the hash value as a lowercase hex string. Length depends on the hash type: 8 chars for XXH32, 16 for
XXH3-64/XXH64, 32 for XXH3-128.

## b64digest

```perl
my $b64 = $ctx->b64digest;
```

Returns the hash value as base64-encoded data. Requires [MIME::Base64](https://metacpan.org/pod/MIME%3A%3ABase64).

## clone

```perl
my $copy = $ctx->clone;
```

Creates an independent copy of the hash context. Modifying `$copy` does not affect `$ctx`.

## reset

```
$ctx->reset;
```

Resets the context to its initial state (same seed/secret as when created). The context can then be reused for a new
hash computation.

# SPEED

This module is the fastest xxHash implementation on CPAN and significantly faster than general-purpose hashes.
Benchmarked on Windows x64 with Perl v5.42 (Strawberry, GCC `-O3 -msse2 -msse4.2`) against [Crypt::xxHash](https://metacpan.org/pod/Crypt%3A%3AxxHash) and
standard digest modules. Run `eg/benchmark.pl` to reproduce.

## XXH3 functional interface (MB/s, higher is better)

```
                      16 bytes    1 KB     64 KB     1 MB
------------------------------------------------------------
xxh3_64              235.5     9,007    22,136    23,371
xxh3_128             176.9     7,561    20,191    22,113
Crypt::xxHash xxh64  120.3     5,951    21,531    23,134
Digest::MD5           76.2       777       907       913
```

- **XXH3-64 beats Crypt::xxHash by up to 96%** at small inputs and matches at large inputs
- **XXH3-128 beats Crypt::xxHash by up to 266%** at small inputs (3.6x faster at 16 bytes)
- **XXH3-128 is 10x faster than MD5** at 1KB

## Streaming interface (1MB fed in 64KB chunks)

```
xxh3_64    18,892 MB/s
xxh3_128   18,656 MB/s
xxh64      15,324 MB/s
xxh32       8,272 MB/s
MD5           923 MB/s
```

- **Streaming xxh3\_64 is 11% faster** than Crypt::xxHash streaming (18,892 vs 17,091 MB/s)
- **Streaming xxh3\_128 is 9% faster** (18,656 vs 17,091 MB/s)

## Hex output (functional, 1KB input)

```
xxh3_64_hex           8,324 MB/s
xxh3_128_hex          7,687 MB/s
xxhash64_hex          6,527 MB/s
xxhash32_hex          5,532 MB/s
Crypt::xxh3_64_hex    3,719 MB/s
Digest::MD5::md5_hex    748 MB/s
```

- **Hex functions use manual nibble writes** instead of `sprintf`, eliminating format string parsing
- **xxh3\_64\_hex is 2.2x faster** than Crypt::xxHash's equivalent
- **xxh3\_128\_hex is 2.1x faster** than Crypt::xxHash's 128-bit hex

# LICENSE

xxHash is covered by the BSD license.

Digest::xxHash is covered by the Artistic 2 license.

# AUTHOR

Sanko Robinson [https://github.com/sanko](https://github.com/sanko)

xxHash by Yann Collet.
