/**
 * @file TaskerConfig.h
 * @author Aleksy Walczak
 * @brief 
 * @date 2021-07-01
 * 
 */

#ifndef TASKERCONFIG_H
#define TASKERCONFIG_H


#define TASKER_LOAD_CALCULATIONS                    // comment this line if you want no load calculations
#define TASKER_LOAD_FILTER_BETA 0.997f             // If load changes too rapidly or too slowly, adjust this value (bigger->change is slover). This value is between 0 and 1
#define TASKER_LOAD_FILTER_BETA_COFACTOR 0.003f    // this should be 1 - TASKER_LOAD_FILTER_BETA

#define SLEEP_FUNCTION                              // comment this line if you don't want to use sleep function


#endif
