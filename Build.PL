use strict;
use Module::Build;

my $build = Module::Build->new(
    module_name         => 'Digest::xxHash',
    license             => 'bsd',
    create_readme       => 1,
    configure_requires  => {
        'Module::Build' => '0.19', # xs
    },
    build_requires       => {
        'ExtUtils::CBuilder' => 0,
        'Test::More'         => 0
    },
    requires       => { },
    c_source       => 'src',
    needs_compiler => 1,
    #extra_compiler_flags => ['-Wall -W -Wundef -Wno-implicit-function-declaration'],
    #extra_linker_flags => [ ],
    include_dirs   => ['.'],
    c_source       => ['src'],
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