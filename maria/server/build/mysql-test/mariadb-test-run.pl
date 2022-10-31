#!/usr/bin/env perl
# Call mtr in out-of-source build
$ENV{MTR_BINDIR} = '/root/work/maria/server/build';
chdir('/root/work/maria/server/mysql-test');
exit(system($^X, '/root/work/maria/server/mysql-test/mariadb-test-run.pl', @ARGV) >> 8);
