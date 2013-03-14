
#include <neuralnetworks/tools/RandomGenerator.h>
#include <time.h>
#include <sys/time.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

using namespace Neural;

RandomGenerator Neural::rnd;


RandomGenerator::RandomGenerator() : _seed(getClockTime()) {
	init();
}

RandomGenerator::RandomGenerator(uint64_t seed) : _seed(seed) {
	init();
}

RandomGenerator::~RandomGenerator() {
	gsl_rng_free(_r);
	gsl_rng_free(_rgamma);
	gsl_rng_free(_rgaussian);
}


void RandomGenerator::init() {
	gsl_rng_default_seed = _seed;
	std::stringstream ss;
	ss << "Initialize Random Generator : seed=" << _seed;
	// select random number generator
	_r = gsl_rng_alloc(gsl_rng_mt19937);
	_rgamma = gsl_rng_alloc(gsl_rng_mt19937);
	_rgaussian = gsl_rng_alloc(gsl_rng_mt19937);
	std::cout << ss.str() << std::endl;
}

uint64_t RandomGenerator::getSeed() const {
	return _seed;
}

uint64_t RandomGenerator::getClockTime() {
    struct timespec ts;
    
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
    
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
	return (uint64_t)ts.tv_sec * 1000000LL + (uint64_t)ts.tv_nsec / 1000LL;
}

double RandomGenerator::doubleRand() {
	double res = gsl_rng_uniform(_r);
	return res;
}

double RandomGenerator::doubleRandGamma(double a, double b) {
	double res = gsl_ran_gamma(_rgamma, a, b);
	return res;
}

double RandomGenerator::doubleRandGaussian(double sigma) {
	double res = gsl_ran_gaussian(_rgaussian, sigma);
	return res;
}

