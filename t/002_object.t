use lib 'blib', 'lib';
use Test::More;

use Digest::xxHash;

my $xxhash = new_ok 'Digest::xxHash' => [12345];

$xxhash->add( 'test' );

is $xxhash->digest(), 3834992036, '...->digest( )';
is $xxhash->digest_hex(), 'e49555a4', '...->digest_hex( )';


$xxhash->reset( );
$xxhash->add( 'test' );
$xxhash->add( 'ing' );

is $xxhash->digest(), 4255065319 , '...->digest( )';

done_testing;