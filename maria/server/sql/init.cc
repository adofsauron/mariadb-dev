/* Copyright (c) 2000, 2010, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1335  USA */

/**
  @file

  @brief
  Init and dummy functions for interface with unireg
*/

#include "mariadb.h"
#include "sql_priv.h"
#include "init.h"
#include "mysqld.h"
#include "my_time.h"  // my_init_time
#include "unireg.h"   // SPECIAL_SAME_DB_NAME
#include <m_ctype.h>

void unireg_init(ulong options)
{
  DBUG_ENTER("unireg_init");

  error_handler_hook = my_message_stderr;
  my_disable_async_io = 1; /* aioread is only in shared library */
  wild_many = '%';
  wild_one = '_';
  wild_prefix = '\\'; /* Change to sql syntax */

  current_pid = (ulong)getpid(); /* Save for later ref */
  my_init_time();                /* Init time-functions (read zone) */

  (void)strmov(reg_ext, ".frm");
  reg_ext_length = 4;
  specialflag = SPECIAL_SAME_DB_NAME | options; /* Set options from argv */
  DBUG_VOID_RETURN;
}
