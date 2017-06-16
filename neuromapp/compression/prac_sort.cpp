#include <stdexcept>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include "compression.h"
#include "block.h"
#include "block_sort.h"
using namespace std;


using neuromapp::block;
using neuromapp::cstandard;
using neuromapp::Sorter;
// todo change to variable type, and allocation method
typedef float value_type;
using prac_block = block<value_type,cstandard>;

int main (void) {
    prac_block b1;
    //read data into block from file
    string fname= "data/csv/values_10_a8213bulk.csv";
    ifstream ifile(fname);
    ifile >> b1;
    std::cout << "block is " << b1  << std::endl;
    //perform sort
    //sort(b1.col_begin(1),b1.col_end(1));
    b1.sort_cols();
    std::cout << "block sorted becomes " << b1 << std::endl;
}
