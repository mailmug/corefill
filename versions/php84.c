#include "php.h"
#include "php_corefill.h"
#include "utils.h"

#if PHP_VERSION_ID < 80400

void define_corefill_const_php84(){
    define_corefill_const("CURL_HTTP_VERSION_3", "30");
    define_corefill_const("CURL_HTTP_VERSION_3ONLY", "31");
}

/* Consumes `zv` */
static bool php_is_true(zval *zv)
{
	switch (Z_TYPE_P(zv)) {
		case IS_TRUE:
			return true;
		case IS_FALSE:
			return false;
		default: {
			bool rv = zend_is_true(zv);
			zval_ptr_dtor(zv);
			return rv;
		}
	}
}

/* {{{ Internal function to find an array element for a user closure. */
enum php_array_find_result {
	PHP_ARRAY_FIND_EXCEPTION = -1,
	PHP_ARRAY_FIND_NONE = 0,
	PHP_ARRAY_FIND_SOME = 1,
};

static enum php_array_find_result php_array_find(const HashTable *array, zend_fcall_info fci, zend_fcall_info_cache *fci_cache, zval *result_key, zval *result_value, bool negate_condition)
{
	zend_ulong num_key;
	zend_string *str_key;
	zval retval;
	zval args[2];
	zval *operand;

	if (zend_hash_num_elements(array) == 0) {
		return PHP_ARRAY_FIND_NONE;
	}

	ZEND_ASSERT(ZEND_FCI_INITIALIZED(fci));

	fci.retval = &retval;
	fci.param_count = 2;
	fci.params = args;

	ZEND_HASH_FOREACH_KEY_VAL(array, num_key, str_key, operand) {
		/* Set up the key */
		if (!str_key) {
			ZVAL_LONG(&args[1], num_key);
		} else {
			/* Allows copying the numeric branch, without this branch, into the iteration code
			 * that checks for the packed array flag. */
			ZEND_ASSUME(!HT_IS_PACKED(array));
			ZVAL_STR(&args[1], str_key);
		}

		ZVAL_COPY_VALUE(&args[0], operand);

		zend_result result = zend_call_function(&fci, fci_cache);
		ZEND_ASSERT(result == SUCCESS);

		if (UNEXPECTED(Z_ISUNDEF(retval))) {
			return PHP_ARRAY_FIND_EXCEPTION;
		}

		if (php_is_true(&retval) ^ negate_condition) {
			if (result_value != NULL) {
				ZVAL_COPY_DEREF(result_value, &args[0]);
			}

			if (result_key != NULL) {
				ZVAL_COPY(result_key, &args[1]);
			}

			return PHP_ARRAY_FIND_SOME;
		}
	} ZEND_HASH_FOREACH_END();

	return PHP_ARRAY_FIND_NONE;
}
/* }}} */

/* {{{ Search within an array and returns the first found element value. */
PHP_FUNCTION(array_find)
{
	HashTable *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	php_array_find(array, fci, &fci_cache, NULL, return_value, false);
}
/* }}} */

/* {{{ Search within an array and returns the first found element key. */
PHP_FUNCTION(array_find_key)
{
	HashTable *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	php_array_find(array, fci, &fci_cache, return_value, NULL, false);
}
/* }}} */

/* {{{ Checks if at least one array element satisfies a callback function. */
PHP_FUNCTION(array_any)
{
	HashTable *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_BOOL(php_array_find(array, fci, &fci_cache, NULL, NULL, false) == PHP_ARRAY_FIND_SOME);
}
/* }}} */

/* {{{ Checks if all array elements satisfy a callback function. */
PHP_FUNCTION(array_all)
{
	HashTable *array;
	zend_fcall_info fci;
	zend_fcall_info_cache fci_cache;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY_HT(array)
		Z_PARAM_FUNC(fci, fci_cache)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_BOOL(php_array_find(array, fci, &fci_cache, NULL, NULL, true) == PHP_ARRAY_FIND_NONE);
}
/* }}} */


/* {{{ Perform floating-point exponentiation with IEEE-754 semantics. */
PHP_FUNCTION(fpow)
{
	double base, exponent;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(base)
		Z_PARAM_DOUBLE(exponent)
	ZEND_PARSE_PARAMETERS_END();

	RETURN_DOUBLE(pow(base, exponent));
}
/* }}} */

#endif