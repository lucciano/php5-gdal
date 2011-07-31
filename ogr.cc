//
// ogr.cc
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

#include "php.h"
#include <ogrsf_frmts.h>

PHP_FUNCTION(ogrregisterall)
{
  if (ZEND_NUM_ARGS() != 0) {
    WRONG_PARAM_COUNT;
  }
  OGRRegisterAll();
}

// not present in C++ API
// PHP_FUNCTION(ogrcleanupall)
// {
//   if (ZEND_NUM_ARGS() != 0) {
//     WRONG_PARAM_COUNT;
//   }
//   OGRCleanupAll();
// }

