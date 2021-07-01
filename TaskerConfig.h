/**
 * @file TaskerConfig.h
 * @author Aleksy Walczak
 * @brief 
 * @date 2021-07-01
 * 
 */

#ifndef TASKERCONFIG_H
#define TASKERCONFIG_H

#define TASKER_LOAD_CALCULATIONS 0              // 1 means "load calculations enabled", 0 means disabled "load calculations disabled"
#define TASKER_LOAD_FILTER_BETA 0.997f          // If load changes too rapidly or too slowly, adjust this value (bigger->change is slover)
#define TASKER_LOAD_FILTER_BETA_COFACTOR 0.003f // this should be 1 - TASKER_LOAD_FILTER_BETA

#endif