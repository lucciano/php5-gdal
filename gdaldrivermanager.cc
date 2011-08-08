//
// gdaldrivermanager.cc
//
//
// Copyright (c) 2011, Geonef SAS
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include <gdal.h>
#include <gdal_priv.h>
#include "php.h"
#include "php_gdal.h"
#include "gdaldrivermanager.h"
#include "gdaldriver.h"

zend_class_entry *gdal_gdaldrivermanager_ce;
zend_object_handlers gdaldrivermanager_object_handlers;

//
// PHP stuff
//

void gdaldrivermanager_free_storage(void *object TSRMLS_DC)
{
  char *msg;
  int i;
  php_gdaldrivermanager_object *obj = (php_gdaldrivermanager_object *)object;

  zend_hash_destroy(obj->std.properties);
  FREE_HASHTABLE(obj->std.properties);
  efree(obj);
}

zend_object_value gdaldrivermanager_create_handler(zend_class_entry *type TSRMLS_DC)
{
  zval *tmp;
  zend_object_value retval;

  php_gdaldrivermanager_object *obj =
    (php_gdaldrivermanager_object *)emalloc(sizeof(php_gdaldrivermanager_object));
  memset(obj, 0, sizeof(php_gdaldrivermanager_object));
  obj->std.ce = type;

  ALLOC_HASHTABLE(obj->std.properties);
  zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
  zend_hash_copy(obj->std.properties, &type->default_properties,
                 (copy_ctor_func_t)zval_add_ref,
                 (void *)&tmp, sizeof(zval *));

  retval.handle =
    zend_objects_store_put(obj, NULL,
                           gdaldrivermanager_free_storage, NULL TSRMLS_CC);
  retval.handlers = &gdaldrivermanager_object_handlers;

  return retval;

  //pdo_stmt_construct(stmt, return_value, dbstmt_ce, ctor_args TSRMLS_CC);

}

//
// CLASS METHODS
//

PHP_METHOD(GDALDriverManager, GetDriverCount)
{
  GDALDriverManager *drivermanager;
  php_gdaldrivermanager_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  RETURN_LONG(drivermanager->GetDriverCount());
}

PHP_METHOD(GDALDriverManager, GetDriver)
{
  GDALDriverManager *drivermanager;
  GDALDriver *driver;
  php_gdaldrivermanager_object *obj;
  php_gdaldriver_object *driver_obj;
  int idx;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"l",
                            &idx) == FAILURE) {
    return;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  driver = drivermanager->GetDriver(idx);
  if (!driver) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_gdaldriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  driver_obj = (php_gdaldriver_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  driver_obj->driver = driver;
}

PHP_METHOD(GDALDriverManager, GetDriverByName)
{
  GDALDriverManager *drivermanager;
  GDALDriver *driver;
  php_gdaldrivermanager_object *obj;
  php_gdaldriver_object *driver_obj;
  char *name;
  int name_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &name, &name_len) == FAILURE) {
    return;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  driver = drivermanager->GetDriverByName(name);
  if (!driver) {
    RETURN_NULL();
  }
  if (object_init_ex(return_value, gdal_gdaldriver_ce) != SUCCESS) {
    RETURN_NULL();
  }
  driver_obj = (php_gdaldriver_object*)
    zend_object_store_get_object(return_value TSRMLS_CC);
  driver_obj->driver = driver;
}

PHP_METHOD(GDALDriverManager, RegisterDriver)
{
  GDALDriverManager *drivermanager;
  GDALDriver *driver;
  php_gdaldrivermanager_object *obj;
  php_gdaldriver_object *driver_obj;
  zval *driver_p;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &driver_p, gdal_gdaldriver_ce) == FAILURE) {
    return;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;

  driver_obj = (php_gdaldriver_object*)
    zend_object_store_get_object(driver_p);
  driver = driver_obj->driver;

  RETURN_LONG(drivermanager->RegisterDriver(driver));
}

PHP_METHOD(GDALDriverManager, MoveDriver)
{
  GDALDriverManager *drivermanager;
  GDALDriver *driver;
  php_gdaldrivermanager_object *obj;
  php_gdaldriver_object *driver_obj;
  zval *driver_p;
  int i;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"Ol",
                            &driver_p, gdal_gdaldriver_ce,
                            &i) == FAILURE) {
    return;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;

  driver_obj = (php_gdaldriver_object*)
    zend_object_store_get_object(driver_p);
  driver = driver_obj->driver;

  drivermanager->MoveDriver(driver, i);
}

PHP_METHOD(GDALDriverManager, DeregisterDriver)
{
  GDALDriverManager *drivermanager;
  GDALDriver *driver;
  php_gdaldrivermanager_object *obj;
  php_gdaldriver_object *driver_obj;
  zval *driver_p;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"O",
                            &driver_p, gdal_gdaldriver_ce) == FAILURE) {
    return;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;

  driver_obj = (php_gdaldriver_object*)
    zend_object_store_get_object(driver_p);
  driver = driver_obj->driver;

  drivermanager->DeregisterDriver(driver);
}


PHP_METHOD(GDALDriverManager, AutoLoadDrivers)
{
  GDALDriverManager *drivermanager;
  php_gdaldrivermanager_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  drivermanager->AutoLoadDrivers();
}

PHP_METHOD(GDALDriverManager, AutoSkipDrivers)
{
  GDALDriverManager *drivermanager;
  php_gdaldrivermanager_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  drivermanager->AutoSkipDrivers();
}

PHP_METHOD(GDALDriverManager, GetHome)
{
  GDALDriverManager *drivermanager;
  php_gdaldrivermanager_object *obj;

  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  const char *home = drivermanager->GetHome();
  if (home) {
    RETURN_STRING((char *)home, 1);
  } else {
    RETURN_NULL();
  }
}

PHP_METHOD(GDALDriverManager, SetHome)
{
  GDALDriverManager *drivermanager;
  php_gdaldrivermanager_object *obj;
  char *home;
  int home_len;

  if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, (char*)"s",
                            &home, &home_len) == FAILURE) {
    return;
  }

  obj = (php_gdaldrivermanager_object *)
    zend_object_store_get_object(getThis() TSRMLS_CC);
  drivermanager = obj->drivermanager;
  drivermanager->SetHome(home);
}


//
// PHP stuff
//

function_entry gdaldrivermanager_methods[] = {
  PHP_ME(GDALDriverManager, GetDriverCount, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, GetDriver, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, GetDriverByName, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, RegisterDriver, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, MoveDriver, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, DeregisterDriver, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, AutoLoadDrivers, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, AutoSkipDrivers, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, GetHome, NULL, ZEND_ACC_PUBLIC)
  PHP_ME(GDALDriverManager, SetHome, NULL, ZEND_ACC_PUBLIC)
  {NULL, NULL, NULL}
};

void php_gdal_gdaldrivermanager_startup(INIT_FUNC_ARGS)
{
  zend_class_entry ce;
  INIT_CLASS_ENTRY(ce, "GDALDriverManager", gdaldrivermanager_methods);
  gdal_gdaldrivermanager_ce = zend_register_internal_class(&ce TSRMLS_CC);
  gdal_gdaldrivermanager_ce->create_object = gdaldrivermanager_create_handler;
  memcpy(&gdaldrivermanager_object_handlers,
         zend_get_std_object_handlers(), sizeof(zend_object_handlers));
  gdaldrivermanager_object_handlers.clone_obj = NULL;
}
