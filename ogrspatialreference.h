//
// ogrspatialreference.h
//
//
// Copyright (c) 2011, Geonef SAS
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef PHP_OGRSPATIALREFERENCE_H
#define PHP_OGRSPATIALREFERENCE_H

#include <ogr_spatialref.h>
#include "php.h"

extern zend_class_entry *gdal_ogrspatialreference_ce;

struct php_ogrspatialreference_object {
  zend_object std;
  OGRSpatialReference *spatialreference;
};

void php_gdal_ogrspatialreference_startup(INIT_FUNC_ARGS);

#endif /* PHP_OGRSPATIALREFERENCE_H */
