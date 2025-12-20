#include "php.h"
#include "php_corefill.h"
#include "utils.h"

#if PHP_VERSION_ID < 80300

#include "php83/ext/json/php_json_parser.h"
#include "php83/ext/json/php_json_scanner.h"
#include "Zend/zend_hash.h"

ZEND_API ZEND_COLD void zend_argument_must_not_be_empty_error(uint32_t arg_num)
{
	zend_argument_value_error(arg_num, "must not be empty");
}

void define_corefill_const_php83(){
    // define_corefill_const("CURL_HTTP_VERSION_3", "30");
    // define_corefill_const("CURL_HTTP_VERSION_3ONLY", "31");
}

/* {{{ */
PHP_JSON_API bool php_json_validate_ex(const char *str, size_t str_len, zend_long options, zend_long depth)
{
	php_json_parser parser;
	zval tmp;
	const php_json_parser_methods* parser_validate_methods = php_json_get_validate_methods();
	php_json_parser_init_ex(&parser, &tmp, str, str_len, (int)options, (int)depth, parser_validate_methods);

	if (php_json_yyparse(&parser)) {
		php_json_error_code error_code = php_json_parser_error_code(&parser);
		JSON_G(error_code) = error_code;
		return false;
	}

	return true;
}
/* }}} */

/* {{{ Validates if a string contains a valid json */
PHP_FUNCTION(json_validate)
{
	char *str;
	size_t str_len;
	zend_long depth = PHP_JSON_PARSER_DEFAULT_DEPTH;
	zend_long options = 0;

	ZEND_PARSE_PARAMETERS_START(1, 3)
		Z_PARAM_STRING(str, str_len)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(depth)
		Z_PARAM_LONG(options)
	ZEND_PARSE_PARAMETERS_END();


	if ((options != 0) && (options != PHP_JSON_INVALID_UTF8_IGNORE)) {
		zend_argument_value_error(3, "must be a valid flag (allowed flags: JSON_INVALID_UTF8_IGNORE)");
		RETURN_THROWS();
	}

	if (!str_len) {
		JSON_G(error_code) = PHP_JSON_ERROR_SYNTAX;
		RETURN_FALSE;
	}

	JSON_G(error_code) = PHP_JSON_ERROR_NONE;

	if (depth <= 0) {
		zend_argument_value_error(2, "must be greater than 0");
		RETURN_THROWS();
	}

	if (depth > INT_MAX) {
		zend_argument_value_error(2, "must be less than %d", INT_MAX);
		RETURN_THROWS();
	}

	RETURN_BOOL(php_json_validate_ex(str, str_len, options, depth));
}
/* }}} */



static zend_result parse_context_options(php_stream_context *context, const HashTable *options)
{
	zval *wval, *oval;
	zend_string *wkey, *okey;

	ZEND_HASH_FOREACH_STR_KEY_VAL(options, wkey, wval) {
		ZVAL_DEREF(wval);
		if (wkey && Z_TYPE_P(wval) == IS_ARRAY) {
			if (!HT_IS_PACKED(Z_ARRVAL_P(wval))) {
				ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(wval), okey, oval) {
					if (okey) {
						php_stream_context_set_option(context, ZSTR_VAL(wkey), ZSTR_VAL(okey), oval);
					}
				} ZEND_HASH_FOREACH_END();
			}
		} else {
			zend_value_error("Options should have the form [\"wrappername\"][\"optionname\"] = $value");
			return FAILURE;
		}
	} ZEND_HASH_FOREACH_END();

	return SUCCESS;
}

/* {{{ ZTS-stuff / Globals / Prototypes */

/* sharing globals is *evil* */
static int le_stream_context = FAILURE;

PHPAPI int php_le_stream_context(void)
{
	return le_stream_context;
}
/* }}} */

/* given a zval which is either a stream or a context, return the underlying
 * stream_context.  If it is a stream that does not have a context assigned, it
 * will create and assign a context and return that.  */
static php_stream_context *decode_context_param(zval *contextresource)
{
	php_stream_context *context = NULL;

	context = zend_fetch_resource_ex(contextresource, NULL, php_le_stream_context());
	if (context == NULL) {
		php_stream *stream;

		stream = zend_fetch_resource2_ex(contextresource, NULL, php_file_le_stream(), php_file_le_pstream());

		if (stream) {
			context = PHP_STREAM_CONTEXT(stream);
			if (context == NULL) {
				/* Only way this happens is if file is opened with NO_DEFAULT_CONTEXT
				   param, but then something is called which requires a context.
				   Don't give them the default one though since they already said they
	 			   didn't want it. */
				context = php_stream_context_alloc();
				stream->ctx = context->res;
			}
		}
	}

	return context;
}
/* }}} */


/* {{{ stream_context_set_options */
PHP_FUNCTION(stream_context_set_options)
{
	zval *zcontext = NULL;
	php_stream_context *context;
	HashTable *options;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_RESOURCE(zcontext)
		Z_PARAM_ARRAY_HT(options)
	ZEND_PARSE_PARAMETERS_END();

	/* figure out where the context is coming from exactly */
	if (!(context = decode_context_param(zcontext))) {
		zend_argument_type_error(1, "must be a valid stream/context");
		RETURN_THROWS();
	}

	if (parse_context_options(context, options) == FAILURE) {
		RETURN_THROWS();
	}

	RETURN_TRUE;
}
/* }}} */


ZEND_API bool zend_string_only_has_ascii_alphanumeric(const zend_string *str)
{
	const char *p = ZSTR_VAL(str);
	const char *e = ZSTR_VAL(str) + ZSTR_LEN(str);
	while (p < e) {
		char c = *p++;
		if (UNEXPECTED( c < '0' || c > 'z' || (c < 'a' && c > 'Z') || (c < 'A' && c > '9') ) ) {
			return false;
		}
	}
	return true;
}

/* {{{ str_increment */
PHP_FUNCTION(str_increment)
{
	zend_string *str;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();

	if (ZSTR_LEN(str) == 0) {
		zend_argument_must_not_be_empty_error(1);
		RETURN_THROWS();
	}
	if (!zend_string_only_has_ascii_alphanumeric(str)) {
		zend_argument_value_error(1, "must be composed only of alphanumeric ASCII characters");
		RETURN_THROWS();
	}

	zend_string *incremented = zend_string_init(ZSTR_VAL(str), ZSTR_LEN(str), /* persistent */ false);
	size_t position = ZSTR_LEN(str)-1;
	bool carry = false;

	do {
		char c = ZSTR_VAL(incremented)[position];
		/* We know c is in ['a', 'z'], ['A', 'Z'], or ['0', '9'] range from zend_string_only_has_ascii_alphanumeric() */
		if (EXPECTED( c != 'z' && c != 'Z' && c != '9' )) {
			carry = false;
			ZSTR_VAL(incremented)[position]++;
		} else { /* if 'z', 'Z', or '9' */
			carry = true;
			if (c == '9') {
				ZSTR_VAL(incremented)[position] = '0';
			} else {
				ZSTR_VAL(incremented)[position] -= 25;
			}
		}
	} while (carry && position-- > 0);

	if (UNEXPECTED(carry)) {
		zend_string *tmp = zend_string_alloc(ZSTR_LEN(incremented)+1, 0);
		memcpy(ZSTR_VAL(tmp) + 1, ZSTR_VAL(incremented), ZSTR_LEN(incremented));
		ZSTR_VAL(tmp)[ZSTR_LEN(incremented)+1] = '\0';
		switch (ZSTR_VAL(incremented)[0]) {
			case '0':
				ZSTR_VAL(tmp)[0] = '1';
				break;
			default:
				ZSTR_VAL(tmp)[0] = ZSTR_VAL(incremented)[0];
				break;
		}
		zend_string_efree(incremented);
		RETURN_NEW_STR(tmp);
	}
	RETURN_NEW_STR(incremented);
}
/* }}} */


#endif