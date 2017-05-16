/*
* File: main.cpp aka execute compression app
* Author: Devin Bayly
* Purpose:  this program establishes the command line argument parsing loop
* used for the neuromapp when compression is typed out as option. 
*/

#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
// the includes are relative to the directory above
#include "compression/allocator.h"
#include "compression/exception.h"
#include "compression/block.h"
//? do I need  to use the util error?

/* make namespace alias for program options */
using neuromapp::block;
using neuromapp::cstandard;
namespace po = boost::program_options;
// todo change to variable type, and allocation method
using std_block = block<int,cstandard>;

//todo decide if pointer is right return value 
std_block * gen_block(std::vector<int> vec)
{
    if (vec.size() == 2) {
        std_block * new_block = new std_block(vec[0],vec[1]);// hopefully constructor dishes out enough heap space for this
        return new_block;
    } else if (vec.size() == 1) {
        // only 1d block
        std_block* new_block = new std_block(vec[0]);
        return new_block;
    }

}

//todo include more boost program_options later on
int comp_execute(int argc,char *const argv[])
{
    try {
        //make desc
        po::options_description desc{"Allowed options"};
        //add options
        desc.add_options()
            //register gen_block with the create user option
            ("help","Print out the help screen")
            ("create",po::value< std::vector<int>>()->multitoken(),"The create option");
        //create variable map
        po::variables_map vm;
        po::store(po::parse_command_line(argc,argv,desc),vm);
        //finish function registration with notify
        po::notify(vm);
        //loop over reasonable options
        if (vm.count("help")) {
            std::cout << desc << std::endl;
        }
        if(vm.count("create")){
            std::vector<int> opts;
            opts = vm["create"].as<std::vector <int>>();// this should launch the gen_block function
            std_block * created_block = gen_block(opts);
            std::cout << created_block->memory_allocated() << std::endl;
        }
    }
    catch (po::error &e) {
        std::cerr << e.what()  << std::endl;
    }
    // not quite sure what the return should be in this scenario
    return 0;
}

