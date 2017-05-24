//
// game_info.hpp
//

#ifndef _GAME_INFO_HPP_
#define _GAME_INFO_HPP_

#define MODE_NUM 1
#define rating_to_tier(x) ((x-1000)/500)

constexpr int sizes[MODE_NUM] = {2};

namespace nan2 {
enum GameMode {
  DEATH,
  DEFAULT
};
}
#endif
