/* corefill extension for PHP */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_corefill.h"
#include "php84.h"
#include "php85.h"
#include "utils.h"

 

PHP_RINIT_FUNCTION(corefill)
{
#if defined(ZTS) && defined(COMPILE_DL_COREFILL)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

#if PHP_VERSION_ID < 80400
	define_corefill_const_php84();
#endif

#if PHP_VERSION_ID < 80500
	define_corefill_const_php85();
#endif

	return SUCCESS;
}

PHP_MINFO_FUNCTION(corefill)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "corefill support", "enabled");
	php_info_print_table_end();
}

static const zend_function_entry ext_functions[] = {
	PHP84_CORE_FUNCTIONS
	PHP85_CORE_FUNCTIONS
	ZEND_FE_END
};

zend_module_entry corefill_module_entry = {
	STANDARD_MODULE_HEADER,
	"corefill",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	NULL,							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(corefill),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(corefill),			/* PHP_MINFO - Module info */
	PHP_COREFILL_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_COREFILL
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(corefill)
#endif
