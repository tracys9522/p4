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


int process::pid()const { return _pid;}
int process::process_size()const { return _process_size;}
int process::arrival_time()const { return _arrival_time;}
int process::service_duration()const { return _service_duration;}

ostream &operator <<(ostream &ostr, const process proc)
{
    ostr << "process pid: " << proc.pid() << ", size: " << proc.process_size() << " , arrival: " << proc.arrival_time() << ", service duration: " << proc.service_duration();
    return ostr;
}
