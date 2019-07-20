//
// Created by Administrator on 2019/5/10.
//

#ifndef SJTU_RM2019_WINTER_VERSION_CONFIG_H
#define SJTU_RM2019_WINTER_VERSION_CONFIG_H
#define DEBUG
//#define INFO
#define Windows
//#define Linux
#define ENEMY_BLUE 1
#define ENEMY_RED 2
#define INFANTRY 3
#define BASE 4

#ifdef DEBUG
#define LOG_DEBUG(x) x
#define LOG_DEBUG_MSG(x) std::cout<<x<<std::endl
#else
#define LOG_DEBUG(x)
#define LOG_DEBUG_MSG(x)
#endif

#ifdef INFO
#define LOG_INFO(x) x
#else
#define LOG_INFO(x)
#endif
#endif //SJTU_RM2019_WINTER_VERSION_CONFIG_H
