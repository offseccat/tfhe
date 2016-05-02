#include <cstdlib>
#include <iostream>
#include <random>
#include <cassert>
#include "lwe.h"

using namespace std;

default_random_engine generator;
uniform_int_distribution<Torus32> uniformTorus32_distrib(INT32_MIN, INT32_MAX);

// Gaussian sample centered in message, with standard deviation sigma
EXPORT Torus32 gaussian32(Torus32 message, double sigma){
    //Attention: all the implementation will use the stdev instead of the gaussian fourier param
    normal_distribution<double> distribution(0.,sigma); //TODO: can we create a global distrib of param 1 and multiply by sigma?
    double err = distribution(generator);
    return message + dtot32(err);
}



// from double to Torus32
EXPORT Torus32 dtot32(double d) {
    return int32_t(int64_t((d - int64_t(d))*_two32));
}
// from Torus32 to double
EXPORT double t32tod(Torus32 x) {
    return double(x)/_two32_double;
}


// Used to approximate the phase to the nearest message possible in the message space
// The constant Msize will indicate on which message space we are working (how many messages possible)
EXPORT Torus32 approxPhase(Torus32 phase, int Msize){
    uint64_t interv = (UINT64_C(1)<<63)/Msize; // width of each intervall
    uint64_t half_interval = interv/2; // begin of the first intervall
    uint64_t phase63 = (uint64_t(phase)<<31) + half_interval;
    //floor to the nearest multiples of interv
    phase63 -= phase63%interv;
    //rescale to torus32
    return int32_t(phase63>>31); 
}

// Used to approximate the phase to the nearest message possible in the message space
// The constant Msize will indicate on which message space we are working (how many messages possible)
EXPORT int modSwitchFromTorus32(Torus32 phase, int Msize){
    uint64_t interv = (UINT64_C(1)<<63)/Msize; // width of each intervall
    uint64_t half_interval = interv/2; // begin of the first intervall
    uint64_t phase63 = (uint64_t(phase)<<31) + half_interval;
    //floor to the nearest multiples of interv
    return phase63/interv;
}

// Used to approximate the phase to the nearest message possible in the message space
// The constant Msize will indicate on which message space we are working (how many messages possible)
EXPORT Torus32 modSwitchToTorus32(int mu, int Msize){
    uint64_t interv = (UINT64_C(1)<<63)/Msize; // width of each intervall
    uint64_t phase63 = mu*interv;
    //floor to the nearest multiples of interv
    return phase63>>31;
}
