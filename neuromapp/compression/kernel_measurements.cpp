/* Filename : kernel_measurements.cpp
 * Authors : Devin Bayly, Tim Ewart
 * Organization : University of Arizona, EPFL
 * Purpose : This file exists to assist in determining what level of computation is necessary to make compression routines 
 *          not significantly contribute to the total computation time. We will experiment with varying levels of complexity for 
 *          the calculations starting with simple STREAM like calculations, and gradually working up to Euler method differential 
 *          equation solving techniques.
 * Date : 2017-07-20 
 */

#include <iterator>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include "compressor.h"
#include "allocator.h"
#include "exception.h"
#include "block_sort.h"
#include "block.h"
#include "bit_shifting.h"
#include "timer_tool.h"

using neuromapp::col_sort;
using neuromapp::generate_split_block;
using neuromapp::block;
using neuromapp::Timer;
typedef size_t size_type;


/* Section basic global variable instantiation  */

//TODO add filepath
#define FNAME "trans_data/values_8_a8780trans_bulk.csv"
#define ARRAY_SIZE 100 

/* Section for the computation functions */
template <typename allocator_type>
struct level1_compute {
    double coef,* ptr,* end;
    public:
    level1_compute(double * ptr_arg,double coef_arg,double * end_arg) 
        :coef {coef},ptr{ptr_arg}, end{end_arg} {}
    void operator () (ostream & os) {
        os << " running lvl1 compute ";
        while ( ptr != end) {
            double result = *ptr +coef;
            ptr++;
        }
    }
};




template <typename allocator_type>
struct level2_compute {
    double * ptr, coef, step, * end;
    size_type cols;
    public:
    level2_compute(double * ptr_arg,double coef_arg,double step_arg,size_type cols_arg,double * end_arg) 
        :  ptr{ptr_arg}, coef{coef_arg}, step{step_arg}, cols{cols_arg}, end {end_arg} {}
    void operator () (ostream & os) {
        os << " running lvl2 compute ";
        while (ptr != end) {
            /* now run the computation function on each element */
            double U = *ptr,tau_fac = *ptr + 3*cols;
            double value= U + coef*(1-U)*exp(-step/tau_fac);
            ptr++;
        }
    }
};



template <typename allocator_type>
struct level3_compute {
    double y_initial,  t_initial, step, t_limit;
    public:
    level3_compute (double y_initial_arg, double t_initial_arg,double step_arg,double t_limit_arg) :
        y_initial {y_initial_arg},  t_initial {t_initial_arg}, step {step_arg}, t_limit {t_limit_arg} {}
    /* this is the function that you should edit if you want a different solution */
    double differential (double y,double t) {
        return pow(y,2) + t*30;// totally arbitrary for the moment
    }
    void operator () (ostream & os){
        os << " running lvl3 compute ";
        /* treat the initials as initials for each step, so initial to start, and each step */
        double y_next;
        while (t_initial < t_limit) {
            y_initial = y_initial + differential(y_initial,t_initial) * (step);
            t_initial +=step;
        }
    }
};

/* Section for sampling strategies */

template< typename allocator_type >
void init_array (block<double,allocator_type> * block_array) {
    ifstream blk_file;
    for (int i = 0; i < (int) ARRAY_SIZE; i++) {
        blk_file.open(FNAME);
        block<double,allocator_type> b1;
        blk_file >> b1;
        block_array[i] = b1;
        blk_file.close();
        blk_file.clear();
    }
}


/* selects a novel position in a representative array  that hasn't been picked already */
template <size_type SIZE>
class Block_selector {
    int selected_count = 0;
    int places [SIZE];
    public:
    Block_selector () {
        memset(places,0,SIZE*sizeof(int)); //zero out the places
    }
    int operator()() {
        if(selected_count > SIZE/10)
            return -1;
        else {
            int place;
            srand(time(0));// seed the random
            while(places[place = rand() % SIZE]);
            places[place]++;
            selected_count++;
            return place;
        }
    }
};



/* section for the these things combined */
template <typename allocator_type,typename fun_ob>
void kernel_measure(fun_ob & f,ostream & os,block<double,allocator_type> & blk) {
    /*create and start the timer,compress run first */
    Timer time_it; time_it.start();
    os << " compression: " ;
    blk.uncompress();
    f(os);
    blk.compress();
    time_it.end();
    os << " duration " << time_it.duration() ;
    blk.uncompress();
    /* now the non-compress run */
    os << " non-compress: " ;
    time_it.start();
    f(os);
    time_it.end();
    os << " duration " << time_it.duration() << std::endl;
}


/* this function allows us to capture compress and non-compress runs using kernel_measure and the different levels of compute complexity */
template <typename allocator_type>
void option_coordinator(ostream & out, double coef,double step,double y_initial, double t_initial,double t_limit) {
    Block_selector<(size_type) ARRAY_SIZE> bs;
    block<double,allocator_type> block_array[ARRAY_SIZE];
    init_array(block_array);
    /* loop variables */
    int pos,count = 0;
    /* continue to select blocks in positions, until 10% of the array has been used. atwhich point the -1 is returned */
    while((pos = bs()) != -1) {
        out << " position is : " << pos<< std::endl; 
        size_type cols = block_array[pos].dim0();
        double * block_ptr = block_array[pos].begin(), * end = block_ptr + cols;
        /* generate the initial computation states for functions */
        level1_compute<allocator_type> f_lvl1(block_ptr,coef,end);
        level2_compute<allocator_type> f_lvl2(block_ptr,coef,step,cols,end);
        level3_compute<allocator_type> f_lvl3(y_initial,t_initial,step,t_limit);
        kernel_measure(f_lvl1,out,block_array[pos]);
        kernel_measure(f_lvl2,out,block_array[pos]);
        kernel_measure(f_lvl3,out,block_array[pos]);
    }
}

int main () {
    ofstream out("kernel_measure.log");
    double t_limit = 1000.0;
    double count = 0;
    for ( double y_param = 0.0 ; y_param < 10.0; y_param += 1.0) {
        for (double step = 0.001;step < 10; step *= 10) {
            std::cout << "progress: " <<setprecision(3) << 100*count++/(10*4) << "%" << std::endl;
            out << "y_param " << y_param << " t_limit " << t_limit << " step " << step << std::endl;
            option_coordinator<neuromapp::cstandard>(out,2.0,step,y_param,0.0,t_limit);
        }
    }
}

