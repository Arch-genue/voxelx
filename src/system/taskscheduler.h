/**
 * @file taskscheduler.h
 * @author Vlad Kartsaev
 * @brief C++ implementation of Task Scheduler
 * @version 0.1
 * @date 2024-02-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include <cstdint>

/**
 * @brief Планировщик задач
 * 
 */
class TaskScheduler {
private:
    uint16_t _size;
    
public:
    TaskScheduler(uint16_t size);
    ~TaskScheduler();

    /**
     * @brief Обновление таймера и запуск задач по таймеру
     * 
     */
    void update();

    void addTask();

    void getTask();
    void setTaskState();
    bool isPaused();
};