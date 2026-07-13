# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- XXH3-64 and XXH3-128 single-shot hashing functions (`xxh3_64`, `xxh3_64_hex`, `xxh3_128_hex`)
- Digest-style OO streaming interface (`new`, `add`, `digest`, `hexdigest`, `b64digest`, `clone`, `reset`) for XXH32, XXH64, XXH3-64, and XXH3-128
- `xxh3_generate_secret` function for custom secret-based XXH3 hashing
- Secret-based hashing via the OO interface (`secret` parameter)

### Changed

- Updated bundled xxHash submodule from v0.6.0 to v0.7.4

### Fixed

- Skip redundant per-call `Math::Int64` load on 64-bit perls (#8)

## [2.04] - 2019-09-05

### Fixed

- Mishandling unsigned 64-bit integers when moving from XS to pure Perl
- Prepend zeros to hex values

## [2.03] - 2016-06-13

### Fixed

- Permissions could cause repeat builds to fail in v2.02

## [2.02] - 2016-06-10

### Changed

- Updated bundled xxHash library to 860118 (June 2nd, 2016)

## [2.01] - 2015-11-09

### Fixed

- Documentation fix

### Changed

- Full support for 64-bit hashes on 32-bit perls built without int64 (basically
  just DWIM Perl)

## [2.00] - 2015-11-04

### Added

- Include 64-bit hash functions

### Changed

- Brought bundled xxHash library up to date (as of 44a6297b)
- **Breaking:** `xxhash(...)` renamed to `xxhash32(...)`
- **Breaking:** `xxhash_hex(...)` renamed to `xxhash32_hex(...)`

### Removed

- Object-oriented interface

## [1.02] - 2013-12-24

### Fixed

- Minor POD tweak

## [1.01] - 2013-09-24

### Fixed

- U16 redef errors
- POD fixes

## [1.00] - 2013-09-22

### Added

- Object-oriented interface

## [0.99] - 2013-09-21

### Added

- Initial release
