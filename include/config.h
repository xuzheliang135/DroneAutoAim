//
// Created by Administrator on 2019/5/10.
//

#ifndef SJTU_RM2019_WINTER_VERSION_CONFIG_H
#define SJTU_RM2019_WINTER_VERSION_CONFIG_H
#define DEBUG
//#define Windows
#define Linux
#define ENEMY_COLOR ENEMY_BLUE

#ifdef DEBUG
#define LOG_DEBUG(x) x
#define LOG_DEBUG_MSG(x) std::cout<<x<<std::endl
#else
#define LOG_DEBUG(x)
#endif

#ifdef INFO
#define LOG_INFO(x) x
#else
#define LOG_INFO(x)
#endif
#endif //SJTU_RM2019_WINTER_VERSION_CONFIG_H
