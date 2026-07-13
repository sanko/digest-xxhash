use strict;
use Config;
use Module::Build;

my $class = Module::Build->subclass(
    override => 'process_support_files',
    code     => <<'END_CODE',
sub process_support_files {
    my $s = shift;
    my $p = $s->{properties};
    return unless $p->{c_source};
    return if $s->pureperl_only && $s->allow_pureperl;

    require Config;
    my $is_x86 = $Config::Config{archname} =~ /x86|i[3-6]86|x64|AMD64|amd64/;
    my $exclude = $is_x86
        ? qr{[/\\](?:tests|cli|fuzz)[/\\]}
        : qr{[/\\](?:tests|cli|fuzz)[/\\]|xxh_x86dispatch\.c$};

    my $files;
    if (ref($p->{c_source}) eq "ARRAY") {
        push @{$p->{include_dirs}}, @{$p->{c_source}};
        for my $path (@{$p->{c_source}}) {
            my @found = grep { !/$exclude/ } @{ $s->rscan_dir($path, $s->file_qr('\.c(c|p|pp|xx|\+\+)?$')) };
            push @$files, @found;
        }
    }
    else {
        push @{$p->{include_dirs}}, $p->{c_source};
        $files = [ grep { !/$exclude/ } @{ $s->rscan_dir($p->{c_source}, $s->file_qr('\.c(c|p|pp|xx|\+\+)?$')) } ];
    }

    push @{$p->{objects}}, $s->compile_c($_) for @$files;
}
END_CODE
);

my $build = $class->new(
    module_name        => 'Digest::xxHash',
    license            => 'bsd',
    create_readme      => 1,
    configure_requires => {
        'Module::Build' => '0.4229'    # xs
    },
    build_requires => { 'ExtUtils::CBuilder' => 0, 'Test::More' => 0 },
    requires       => {
        'perl'        => '5.022',
        ( $Config::Config{ivsize} < 8 ? ( 'Math::Int64' => '0.53' ) : () ),
    },
    needs_compiler => 1,

    #extra_compiler_flags => ['-Wall -W -Wundef -Wno-implicit-function-declaration'],
    #extra_linker_flags => [ ],
    include_dirs => ['.'],
    c_source     => [ 'ext/xxHash', 'ext/perl_math_int64'],
    xs_files     => { './xxHash.xs' => 'lib/Digest/xxHash.xs' },
    meta_merge   =>
        { resources => { bugtracker => 'https://github.com/sanko/digest-xxhash/issues', repository => 'https://github.com/sanko/digest-xxhash' } }
);
$build->create_build_script;
