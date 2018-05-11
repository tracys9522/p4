#ifndef PROCESS_H
#define PROCESS_H
#include <ostream>
class process
{
  int _pid;
  int _process_size;
  int _arrival_time;
  int _service_duration;

public:
  process();
  process(int pid, int process_size, int arrival_time, int service_duration);

};
std::ostream &operator <<(std::ostream &ostr, const process proc);
#endif
