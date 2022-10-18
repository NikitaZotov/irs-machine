/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <websocketpp/server.hpp>
#include <websocketpp/config/core.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <set>

using IrsServerPort = size_t;
using IrsServerCore = websocketpp::server<websocketpp::config::asio>;
using IrsServerConnectionHandle = websocketpp::connection_hdl;
using IrsServerConnections = std::set<IrsServerConnectionHandle, std::owner_less<IrsServerConnectionHandle>>;

using IrsServerMessage = IrsServerCore::message_ptr;
using IrsServerMessageType = websocketpp::frame::opcode::value;
using IrsServerCloseCode = websocketpp::close::status::value;

using IrsServerException = websocketpp::exception;

using IrsServerLogChannel = websocketpp::log::level;
using IrsServerLogMessages = websocketpp::log::alevel;
using IrsServerLogErrors = websocketpp::log::elevel;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::placeholders::_3;

#define IRS_SERVER_DEBUG_LEVEL "Debug"
#define IRS_SERVER_INFO_LEVEL "Info"
#define IRS_SERVER_ERROR_LEVEL "Error"

#define IRS_SERVER_CONSOLE_TYPE "Console"
#define IRS_SERVER_FILE_TYPE "File"

