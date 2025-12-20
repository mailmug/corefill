/* corefill extension for PHP */

#ifndef PHP_COREFILL_H
# define PHP_COREFILL_H

extern zend_module_entry corefill_module_entry;
# define phpext_corefill_ptr &corefill_module_entry

# define PHP_COREFILL_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_COREFILL)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_COREFILL_H */
