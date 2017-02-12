/* *
 *
 * File:  utils.cpp
 * Brief: Miscellaneous functions
 * 
 * Copyright (C) 2016-2017 Matyas Cerny <cerny-matyas(at)email.cz>
 *
 *
 * This file is part of Dun-geon.
 *
 * Dun-geon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dun-geon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dun-geon.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#include "utils.h"
#include <random>
#include <ctime>
#include <fstream>

std::mt19937 mt(std::time(0));

int RNG(int low, int high) {
	std::uniform_int_distribution<int> randNum(low, high);
	return randNum(mt);
}

bool fileExists(const std::string &filename) {
	std::ifstream f(filename.c_str());
	return f.good();
}
