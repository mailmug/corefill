php=php@8.0
PHP_PREFIX=$(brew --prefix $php)

$PHP_PREFIX/bin/phpize --clean
$PHP_PREFIX/bin/phpize

./configure --with-php-config=$PHP_PREFIX/bin/php-config

make clean
make -j$(sysctl -n hw.ncpu)

$PHP_PREFIX/bin/php -dextension=modules/corefill.so  z.php   

make test