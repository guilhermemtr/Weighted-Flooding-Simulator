#include <sys/sysinfo.h>

#define num_cpus() get_nprocs_conf ()


#ifdef _OPENMP
#include <omp.h>
#endif
