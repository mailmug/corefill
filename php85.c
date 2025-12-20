#include "php.h"
#include "php_corefill.h"
#include "utils.h"

#if PHP_VERSION_ID < 80500

#define RETURN_COPY_DEREF(zv)			do { RETVAL_COPY_DEREF(zv); return; } while (0)
#define RETVAL_COPY_DEREF(zv)			ZVAL_COPY_DEREF(return_value, zv)

void define_corefill_const_php85(){
    // define_corefill_const("CURL_HTTP_VERSION_3", "30");
    // define_corefill_const("CURL_HTTP_VERSION_3ONLY", "31");
}

ZEND_FUNCTION(get_exception_handler)
{
	ZEND_PARSE_PARAMETERS_NONE();

	if (Z_TYPE(EG(user_exception_handler)) != IS_UNDEF) {
		RETURN_COPY(&EG(user_exception_handler));
	}
}

ZEND_FUNCTION(get_error_handler)
{
	ZEND_PARSE_PARAMETERS_NONE();

	if (Z_TYPE(EG(user_error_handler)) != IS_UNDEF) {
		RETURN_COPY(&EG(user_error_handler));
	}
}


PHP_FUNCTION(array_first)
{
	HashTable *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(array)
	ZEND_PARSE_PARAMETERS_END();

	ZEND_HASH_FOREACH_VAL(array, zval *zv) {
		RETURN_COPY_DEREF(zv);
	} ZEND_HASH_FOREACH_END();
}

PHP_FUNCTION(array_last)
{
	HashTable *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_HT(array)
	ZEND_PARSE_PARAMETERS_END();

	ZEND_HASH_REVERSE_FOREACH_VAL(array, zval *zv) {
		RETURN_COPY_DEREF(zv);
	} ZEND_HASH_FOREACH_END();
}


#endif