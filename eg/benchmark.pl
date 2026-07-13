use v5.42;
use Time::HiRes qw[clock_gettime CLOCK_MONOTONIC];
use Config;
no warnings qw[experimental::builtin experimental::try];
$|++;
#
use blib;
use Digest::xxHash qw[
    xxhash32 xxhash32_hex xxhash64 xxhash64_hex
    xxh3_64  xxh3_64_hex
    xxh3_128 xxh3_128_hex
];
#
my %HAVE;
for my $mod (qw[Digest::CRC Digest::JHash Crypt::xxHash]) {
    try {
        $HAVE{$mod} = builtin::load_module $mod
    }
    catch ($e) {

        # say 'Failed to load '. $mod . ' which is okay.'
    }
}

# Crypto hashes for reference
use Digest::MD5 ();
use Digest::SHA ();

# Helpers
sub banner ($line) { say "\n", '=' x 76, "\n  $line\n", '=' x 76 }

sub size_label ($s) {
    return "${s}B" if $s < 1024;
    return sprintf( '%.0fKB', $s / 1024 ) if $s < 1024 * 1024;
    return sprintf( '%.1fMB', $s / ( 1024 * 1024 ) );
}

sub time_fn ( $fn, $min_iters, $sec ) {
    $sec //= 2.0;
    $fn->() for 1 .. 10;    # warm up
    my $iters = $min_iters;
    my $start = clock_gettime(CLOCK_MONOTONIC);
    $fn->() for 1 .. $iters;
    my $elapsed = clock_gettime(CLOCK_MONOTONIC) - $start;
    if ( $elapsed < $sec && $elapsed > 0 ) {
        my $scale = int( $sec / $elapsed ) + 1;
        $iters *= $scale;
        $start = clock_gettime(CLOCK_MONOTONIC);
        $fn->() for 1 .. $iters;
        $elapsed = clock_gettime(CLOCK_MONOTONIC) - $start;
    }
    return ( $iters, $elapsed );
}

sub mbps ( $bytes, $iters, $elapsed ) {
    return 0 if $elapsed <= 0;
    return ( $bytes * $iters ) / ( $elapsed * 1024 * 1024 );
}

sub bench_group ( $label, $data_size, $hashref ) {
    say "\n  [$label]";
    my @results;
    my $best = 0;
    for my $name ( sort keys %$hashref ) {
        my $min_iters = $data_size <= 1024 ? 100_000 : $data_size <= 64 * 1024 ? 10_000 : 1_000;
        my ( $iters, $elapsed ) = time_fn( $hashref->{$name}, $min_iters, 2.0 );
        my $rate = mbps( $data_size, $iters, $elapsed );
        push @results, [ $name, $rate ];
        $best = $rate if $rate > $best;
    }
    for my $row ( sort { $b->[1] <=> $a->[1] } @results ) {
        say sprintf '  %-34s %10.1f MB/s%s', $row->[0], $row->[1], $row->[1] == $best ? ' <<<' : '';
    }
}
#
say 'Digest::xxHash Benchmark';
say "Perl $^V | $Config{archname} | ivsize=$Config{ivsize}bit";
say '-' x 76;
say 'Comparators:';
say '  - Crypt::xxHash'       if $HAVE{'Crypt::xxHash'};
say '  - Digest::CRC (CRC32)' if $HAVE{'Digest::CRC'};
say '  - Digest::JHash'       if $HAVE{'Digest::JHash'};
say '  - Digest::MD5';
say '  - Digest::SHA (SHA-224)';
#
for my $size ( 16, 256, 1024, 64 * 1024, 1024 * 1024 ) {
    my $data  = 'A' x $size;
    my $label = size_label($size);
    #
    banner("Input: $label ($size bytes)");

    # 32-bit
    my %h32 = ( 'Digest::xxHash xxhash32' => sub { xxhash32( $data, 0 ) } );
    $h32{'Crypt::xxHash xxhash32'} = sub { Crypt::xxHash::xxhash32( $data, 0 ) }
        if $HAVE{'Crypt::xxHash'};
    $h32{'Digest::CRC crc32'} = sub { Digest::CRC::crc32($data) }
        if $HAVE{'Digest::CRC'};
    bench_group( '32-bit output', $size, \%h32 );

    # 64-bit
    my %h64 = ( 'Digest::xxHash xxhash64' => sub { xxhash64( $data, 0 ) }, 'Digest::xxHash xxh3_64' => sub { xxh3_64( $data, 0 ) }, );
    $h64{'Crypt::xxHash xxhash64'} = sub { Crypt::xxHash::xxhash64( $data, 0 ) }
        if $HAVE{'Crypt::xxHash'};
    $h64{'Crypt::xxHash xxhash3_64bits'} = sub { Crypt::xxHash::xxhash3_64bits( $data, 0 ) }
        if $HAVE{'Crypt::xxHash'};
    bench_group( '64-bit output', $size, \%h64 );

    # 128-bit
    my %h128 = (
        'Digest::xxHash xxh3_128' => sub { xxh3_128( $data, 0 ) },
        'Digest::MD5 md5'         => sub { Digest::MD5::md5($data) },
        'Digest::SHA sha224'      => sub { Digest::SHA::sha224($data) }
    );
    $h128{'Crypt::xxHash xxhash3_128bits_hex'} = sub { Crypt::xxHash::xxhash3_128bits_hex( $data, 0 ) }
        if $HAVE{'Crypt::xxHash'};
    bench_group( '128-bit class output', $size, \%h128 );
}
banner('Streaming interface - 1MB fed in 64KB chunks');
{
    my $chunk  = 'B' x ( 64 * 1024 );
    my $nchunk = 16;
    my $size   = 1024 * 1024;
    my %stream = (
        'Digest::xxHash xxh3_64' => sub {
            my $ctx = Digest::xxHash->new( type => 'xxh3_64' );
            $ctx->add($chunk) for 1 .. $nchunk;
            $ctx->hexdigest;
        },
        'Digest::xxHash xxh3_128' => sub {
            my $ctx = Digest::xxHash->new( type => 'xxh3_128' );
            $ctx->add($chunk) for 1 .. $nchunk;
            $ctx->hexdigest;
        },
        'Digest::xxHash xxh32' => sub {
            my $ctx = Digest::xxHash->new( type => 'xxh32' );
            $ctx->add($chunk) for 1 .. $nchunk;
            $ctx->hexdigest;
        },
        'Digest::xxHash xxh64' => sub {
            my $ctx = Digest::xxHash->new( type => 'xxh64' );
            $ctx->add($chunk) for 1 .. $nchunk;
            $ctx->hexdigest;
        },
        'Digest::MD5' => sub {
            my $ctx = Digest::MD5->new;
            $ctx->add($chunk) for 1 .. $nchunk;
            $ctx->hexdigest;
        },
        'Digest::SHA sha224' => sub {
            my $ctx = Digest::SHA->new(224);
            $ctx->add($chunk) for 1 .. $nchunk;
            $ctx->hexdigest;
        }
    );
    if ( $HAVE{'Crypt::xxHash'} ) {
        $stream{'Crypt::xxHash xxh3_64bits_stream'} = sub {
            my $s = Crypt::xxHash::xxhash3_64bits_stream(0);
            Crypt::xxHash::xxhash3_64bits_stream_update( $s, $chunk ) for 1 .. $nchunk;
            Crypt::xxHash::xxhash3_64bits_stream_digest_hex($s);
        };
    }
    bench_group( 'Streaming, 1MB total', $size, \%stream );
}
banner("Hex digest overhead (functional, 1KB)");
{
    my $data = 'C' x 1024;
    my %hex  = (
        'Digest::xxHash xxhash32_hex' => sub { xxhash32_hex( $data, 0 ) },
        'Digest::xxHash xxhash64_hex' => sub { xxhash64_hex( $data, 0 ) },
        'Digest::xxHash xxh3_64_hex'  => sub { xxh3_64_hex( $data, 0 ) },
        'Digest::xxHash xxh3_128_hex' => sub { xxh3_128_hex( $data, 0 ) },
        'Digest::MD5 md5_hex'         => sub { Digest::MD5::md5_hex($data) },
        'Digest::SHA sha224_hex'      => sub { Digest::SHA::sha224_hex($data) }
    );
    if ( $HAVE{'Crypt::xxHash'} ) {
        $hex{'Crypt::xxHash xxhash32_hex'}        = sub { Crypt::xxHash::xxhash32_hex( $data, 0 ) };
        $hex{'Crypt::xxHash xxhash64_hex'}        = sub { Crypt::xxHash::xxhash64_hex( $data, 0 ) };
        $hex{'Crypt::xxHash xxhash3_64bits_hex'}  = sub { Crypt::xxHash::xxhash3_64bits_hex( $data, 0 ) };
        $hex{'Crypt::xxHash xxhash3_128bits_hex'} = sub { Crypt::xxHash::xxhash3_128bits_hex( $data, 0 ) };
    }
    bench_group( 'Hex output, 1KB input', 1024, \%hex );
}
say '-' x 76;
say 'Legend: <<< = fastest in group';
say 'Digest::xxHash = this module | Crypt::xxHash = independent CPAN implementation';
