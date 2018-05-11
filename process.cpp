#include "process.h"

process::process()
{
    _pid = -1;
}

process::process(int pid, int process_size, int arrival_time, int service_duration)
{
    _pid = pid;
    _process_size = process_size;
    _arrival_time = arrival_time;
    _service_duration = service_duration;
}


int process::get_arrival_time(){
    return _arrival_time;
}

std::ostream &operator <<(std::ostream &ostr, const process proc)
{
    //std::cout << "process pid: " << proc._pid << ", size: " << proc._process_size << " , arrival: " << proc._arrival_time << ", service duration: " << proc._service_duration << std::endl;
    return ostr;
}
