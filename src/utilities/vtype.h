/**
 * @file vtype.h
 * @author Vlad Kartsaev
 * @brief Voxel3d types
 * @version 0.1
 * @date 2024-03-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <algorithm>
#include <vector>

namespace vtype {
    /**
     * @brief Вектор с возможностью поиска по значению  
     * 
     * @tparam T Тип
     */
    template<typename T>
    class fndvector {
    private:
        std::vector<T> _data;

    public:
        void push_back(const T& value) {
            _data.push_back(value);
        }

        T at(size_t n) {
            return _data[n];
        }

        bool contains(const T &value) const {
            return std::find(_data.begin(), _data.end(), value) != _data.end();
        }

        int32_t indexOf(const T &value) {
            auto it = std::find(_data.begin(), _data.end(), value);
            if (it != _data.end()) {
                return std::distance(_data.begin(), it);
            } else {
                return -1;
            }
        }

        size_t size() const {
            return _data.size();
        }

        void clear() {
            _data.clear();
        }
    };

    /**
     * @brief Трехмерный массив
     * 
     * @tparam X Тип индекса
     * @tparam T Тип значения
     */
    template<typename X, typename T>
    class array3 {
    private:
        X*** _valueind;
        T* _value;

        int16_t _sizex;
        int16_t _sizey;
        int16_t _sizez;

        size_t _incr;

        size_t _sumsize;
    public:
        array3() {
            #pragma comment ARRAY3 WITHOUT SIZE!!!
        }
        array3(int16_t &sizex, int16_t &sizey, int16_t &sizez) {
            _sizex = sizex;
            _sizey = sizey;
            _sizez = sizez;

            _incr = 0;

            _sumsize = sizex * sizey * sizez;

            _valueind = new X**[sizex];

            for (int i = 0; i < sizex; ++i) {
                _valueind[i] = new X*[sizey];
                for (int j = 0; j < sizey; ++j) {
                    _valueind[i][j] = new X[sizez];
                    for (int k = 0; k < sizez; ++k) {
                        _valueind[i][j][k] = -1;
                    }
                }
            }
            _value = new T[_sumsize];
        }
        ~array3() {}

        /**
         * @brief Добавить элемент в массив
         * 
         * @param x 
         * @param y 
         * @param z 
         * @param value Значение для добавления
         * @return Индекс этого элемента
         */
        size_t add(X &x, X &y, X &z, T value) {
            _value[_incr] = value;
            _valueind[x][y][z] = _incr;
            _incr++;

            return _incr;
        }

        size_t size() {
            return _sumsize;
        }

        void assign(X &x, X &y, X &z, T value) {
            _value[_valueind[x][y][z]] = value;
        }

        T get(X &x, X &y, X &z) {
            int ind = _valueind[x][y][z];
            if (ind == -1) {
                return nullptr;
            }
            return _value[ind];
        }
    };
}