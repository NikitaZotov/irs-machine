/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef _irs_io_h_
#define _irs_io_h_

#include <glib.h>

#include "../../irs-base/irs_types.h"

typedef GIOChannel irs_io_channel;

/// io statuses
#define IRS_IO_STATUS_NORMAL G_IO_STATUS_NORMAL

/// seek types
#define IRS_IO_SEEK_SET G_SEEK_SET

#define irs_io_new_channel(file_path, mode, errors) g_io_channel_new_file(file_path, mode, errors)

#define irs_io_channel_set_encoding(channel, encoding, errors) g_io_channel_set_encoding(channel, encoding, errors)

#define irs_io_channel_shutdown(channel, flush, errors) g_io_channel_shutdown(channel, flush, errors)

#define irs_io_channel_write_chars(channel, chars, count, written_bytes, errors) \
  g_io_channel_write_chars(channel, chars, count, written_bytes, errors)

#define irs_io_channel_seek(channel, offset, type, errors) g_io_channel_seek_position(channel, offset, type, errors)

#endif
