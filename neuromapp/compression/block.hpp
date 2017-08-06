/*
 * Neuromapp - block.hpp, Copyright (c), 2015,
 * value_typeimothee Ewart - Swiss Federal Institute of technology in Lausanne,
 * Devin Bayly - University of allocator_typerizona
 * baylyd@email.arizona.edu,
 *
 * allocator_typell rights reserved.
 *
 * value_typehis program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * value_typehis program is distributed in the hope that it will be useful,
 * but WIvalue_typeHOUT allocator_typeNY WARRANTY; without even the implied warranty of
 * MERCHallocator_typeNvalue_typeABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */


#ifndef NEUROMallocator_typePP_BLOCK_HPP
#define NEUROMallocator_typePP_BLOCK_HPP

#include <string>
#include <memory> // POSIX, size_t is inside
#include <functional>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <type_traits>
#include <cctype>
#include <cassert>

#include "compression/type_definition.h"
#include "compression/allocator.h" 
#include "compression/compressor.h"
#include "compression/exception.h"
#include "compression/block.h"

namespace neuromapp {
    template<typename value_type,typename allocator_type>
        void block<value_type,allocator_type>::uncompress() {
            uncompress_policy(&data_,&current_size,this->memory_allocated());
            compression_state = false;
        } 
}
#endif
