/*
 * configuration.hpp
 * Copyright (C) 2020 Author removed for double-blind evaluation
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONFIG_BWT
#define CONFIG_BWT

#include <map>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <sdsl/vectors.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <sdsl/wt_algorithm.hpp>
#include <set>
#include <tuple>
#include <vector>

using namespace sdsl;
using namespace std;

enum state_type { s = 0, p = 1, o = 2 };
// enum state_type{s, p, o};
// typedef sdsl::wm_int<bit_vector, rank_support_v<>, select_support_scan<>,
// select_support_scan<>> bwt_type;
/*typedef sdsl::wm_int<bit_vector> bwt_type;


typedef sdsl::bit_vector C_type;
typedef sdsl::rank_support_v<> C_rank_type;
typedef sdsl::select_support_mcl<> C_select_type;
typedef sdsl::select_support_mcl<0> C_select0_type;*/

#endif
