#if PHP_VERSION_ID < 80300

void define_corefill_const_php83();

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_json_validate, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, depth, IS_LONG, 0, "512")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, flags, IS_LONG, 0, "0")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stream_context_set_options, 0, 2, IS_TRUE, 0)
	ZEND_ARG_INFO(0, context)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_str_increment, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(json_validate);
ZEND_FUNCTION(stream_context_set_options);
ZEND_FUNCTION(str_increment);


#define PHP83_CORE_FUNCTIONS \
	ZEND_FE(json_validate, arginfo_json_validate) \
	ZEND_FE(stream_context_set_options, arginfo_stream_context_set_options) \
	ZEND_FE(str_increment, arginfo_str_increment)

#endif