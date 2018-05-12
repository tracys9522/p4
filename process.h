#ifndef PROCESS_H
#define PROCESS_H
#include <ostream>
using namespace std;
class process
{
  int _pid;
  int _process_size;
  int _arrival_time;
  int _service_duration;

public:
  process();
  process(int pid, int process_size, int arrival_time, int service_duration);

  int pid()const;
  int process_size()const;
  int arrival_time()const;
  int service_duration()const;

};
ostream &operator <<(ostream &ostr, const process proc);
#endif
