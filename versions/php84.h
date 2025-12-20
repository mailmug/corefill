#if PHP_VERSION_ID < 80400


void define_corefill_const_php84();


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_find, 0, 2, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_array_find_key arginfo_array_find

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_any, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
ZEND_END_ARG_INFO()

#define arginfo_array_all arginfo_array_any

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_fpow, 0, 2, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, num, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, exponent, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()


ZEND_FUNCTION(array_find);
ZEND_FUNCTION(array_find_key);
ZEND_FUNCTION(array_any);
ZEND_FUNCTION(array_all);
ZEND_FUNCTION(fpow);


#define PHP84_CORE_FUNCTIONS \
	ZEND_FE(array_find, arginfo_array_find) \
	ZEND_FE(array_find_key, arginfo_array_find_key) \
	ZEND_FE(array_any, arginfo_array_any) \
	ZEND_FE(array_all, arginfo_array_all) \
	ZEND_FE(fpow, arginfo_fpow) \

#endif

