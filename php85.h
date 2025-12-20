#if PHP_VERSION_ID < 80500

void define_corefill_const_php85();

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_array_first, 0, 1, IS_MIXED, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_array_last arginfo_array_first

#define arginfo_get_exception_handler arginfo_get_error_handler

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_get_error_handler, 0, 0, IS_CALLABLE, 1)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(get_error_handler);
ZEND_FUNCTION(get_exception_handler);
ZEND_FUNCTION(array_first);
ZEND_FUNCTION(array_last);

#define PHP85_CORE_FUNCTIONS \
	ZEND_FE(get_error_handler, arginfo_get_error_handler) \
	ZEND_FE(get_exception_handler, arginfo_get_exception_handler) \
    ZEND_FE(array_first, arginfo_array_first) \
    ZEND_FE(array_last, arginfo_array_last) 

#endif