#ifndef USER_ACTION_HPP
#define USER_ACTION_HPP

struct UserAction {
  int user_id;
  int type;  // 1: join, 2: leave, 3456: move LRUD
  float arg;
};

#endif
