use strict;
use Module::Build;

# Annoyingly with 'c_source' Module::Build can *only* add entire directory
# *trees* recursively. This (no longer) works for xxHash, because their checkout
# now contains a subdirectory with test programs, including multiple definitions
# of main(), and the naive Module::Build approach attempts to link all object
# files into one shared library, which isn't going to work in that case.
# So we have to hack it. Grrr.
my $class = Module::Build->subclass (
    class => 'My::Builder',
    code => <<'EOC',

sub process_support_files {
    my $self = shift;
    my $p = $self->{properties};
    push @{$p->{objects}}, $self->compile_c('ext/xxHash/xxhash.c');
    return $self->SUPER::process_support_files(@_);
}

EOC
);

my $build = $class->new(
    module_name         => 'Digest::xxHash',
    license             => 'bsd',
    create_readme       => 1,
    configure_requires  => {
        'Module::Build' => '0.4229' # xs
    },
    build_requires       => {
        'ExtUtils::CBuilder' => 0,
        'Test::More'         => 0
    },
    requires       => {
		'Math::Int64' => '0.53' # 32bit perl without int64 :\
	},
    needs_compiler => 1,
    #extra_compiler_flags => ['-Wall -W -Wundef -Wno-implicit-function-declaration'],
    #extra_linker_flags => [ ],
    include_dirs   => ['.', 'ext/xxHash'],
    c_source       => ['ext/perl_math_int64'],
    xs_files       => {
        './xxHash.xs' => 'lib/Digest/xxHash.xs' 
    },
    meta_merge     => {
        resources => {
            bugtracker  => 'https://github.com/sanko/digest-xxhash/issues',
            repository  => 'https://github.com/sanko/digest-xxhash'
        }
    }
);

$build->create_build_script;
