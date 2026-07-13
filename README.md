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

## xxhash32( $data, $seed )

Calculates a 32-bit hash and returns it as an unsigned 32-bit integer.

## xxhash32\_hex( $data, $seed )

Calculates a 32-bit hash and returns it as an 8-character lowercase hex string.

## xxhash64( $data, $seed )

Calculates a 64-bit hash and returns it as an unsigned 64-bit integer.

## xxhash64\_hex( $data, $seed )

Calculates a 64-bit hash and returns it as a 16-character lowercase hex string.

## xxh3\_64( $data, $seed )

Calculates a 64-bit XXH3 hash and returns it as an unsigned 64-bit integer. This is faster than `xxhash64()` on all
platforms.

## xxh3\_64\_hex( $data, $seed )

Calculates a 64-bit XXH3 hash and returns it as a 16-character lowercase hex string.

## xxh3\_128\_hex( $data, $seed )

Calculates a 128-bit XXH3 hash and returns it as a 32-character lowercase hex string.

## xxh3\_generate\_secret( $seed )

Generates a 192-byte high-entropy secret from a seed. The returned bytes can be used as the `secret` parameter in
`new()`. This is equivalent to `XXH3_generateSecret_fromSeed()` in the C API.

# OBJECT-ORIENTED INTERFACE

The OO interface follows the same conventions as [Digest::MD5](https://metacpan.org/pod/Digest%3A%3AMD5) and [Digest::SHA](https://metacpan.org/pod/Digest%3A%3ASHA), making it easy to swap hash
algorithms with minimal code changes.

## new

```perl
my $ctx = Digest::xxHash->new( type => 'xxh3_64', seed => 42 );
```

Creates a new hash context. Valid parameters:

- **type** (required) - One of: `xxh32`, `xxh64`, `xxh3_64`, `xxh3_128`
- **seed** (optional) - A numeric seed, defaults to `0`
- **secret** (optional) - Raw bytes to use as a custom secret (XXH3 types only)

When `secret` is provided for XXH3 types, it overrides `seed`. Generate a proper secret with
`xxh3_generate_secret()`.

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

According to the xxHash project's benchmarks on an Intel i7-9700K running Ubuntu x64:

```
Name             Large Data Speed   Small Data Velocity
XXH3-64 (AVX2)        59.4 GB/s              133.1
XXH3-128 (AVX2)       57.9 GB/s              118.1
XXH64                 19.4 GB/s               71.0
XXH32                  9.7 GB/s               71.9
```

# LICENSE

xxHash is covered by the BSD license.

License-wise, I don't actually care about the wrapper I've written.

# AUTHOR

Sanko Robinson [https://github.com/sanko](https://github.com/sanko)

xxHash by Yann Collet.
