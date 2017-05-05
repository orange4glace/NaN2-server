//
// header.hpp 
// for precompiled header
//

#ifndef _HEADER_HPP_
#define _HEADER_HPP_

#include <iostream>
#include <string>
#include <random>
#include <memory>
#include <unordered_map>

#include <mgne/mgne.hpp>
#include <boost/asio.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// -- MySQL --
#include "../../include/mysql.h"
// -----------

// -- Redis --
#include "../../include/redis.h"
// -----------

#include "../../include/models/user.hpp"

#include "game_matching_queue.hpp"
#include "group.hpp"
#include "user_list.hpp"

#include "protocol.hpp"

// -- FlatB --
#include "group_ans_generated.h"
#include "group_ntf_generated.h"
#include "group_req_generated.h"
#include "join_ans_generated.h"
#include "join_req_generated.h"
#include "match_ans_generated.h"
#include "match_ntf_generated.h"
#include "match_req_generated.h"
// ----------


/* git ignored */
#include "../../include/server_db_infos.h"

#endif

