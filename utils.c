#include "php.h"
#include "zend_API.h"
#include "zend_constants.h"

bool define_corefill_const(const char* name, const char *val){
    zend_constant c;
    
    ZEND_CONSTANT_SET_FLAGS(&c, 0, PHP_USER_CONSTANT);
    ZVAL_STRINGL(&c.value, val, strlen(val));

    c.name = zend_string_init(name, strlen(name), 0);
	if (zend_register_constant(&c) == SUCCESS) {
		return true;;
	} 

    zend_string_release(c.name);
    zval_ptr_dtor(&c.value);
    
    return false;
}