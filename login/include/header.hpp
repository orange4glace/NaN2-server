//
// header.hpp 
// for precompiled header
//

#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <iostream>
#include <string>
#include <random>

#include <mgne/mgne.hpp>
#include <boost/scoped_ptr.hpp>

// -- MySQL --
#include "../../include/mysql.h"
// -----------

// -- Redis --
#include "../../include/redis.h"
// -----------

#include "../../include/models/user.hpp"

#include "protocol.hpp"

// -- FlatB --
#include "login_ans_generated.h"
#include "login_req_generated.h"
#include "signup_ans_generated.h"
#include "signup_req_generated.h"
// -----------

/* git ignored */
#include "../../include/server_db_info.h"

#endif

