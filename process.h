#ifndef PROCESS_H
#define PROCESS_H
#include <ostream>
#include <queue>
#include <string>
#include "page.h"

class process
{
  int _pid;
  std::queue<page> _pages;
  // int _page_num;
  int _arrival_time;
  int _running_time;
  int _remaining_time;
  int _last_page_used;
  int _page_size;

public:
  process();
  process(int pid, int page_size, int arrival_time, int running_time);

  int get_pid() const;
  // int get_page_num() const;
  std::queue<page>& get_pages();
  int get_arrival_time() const;
  int get_running_time() const;
  int get_end_time() const;
  int get_last_page_used() const;
  int get_page_size() const;
  std::string str();

  void set_pid(int pid);
  // void set_page_num(int page_num);
  void set_arrival_time(int arrival_time);
  void set_last_page_used(int page_used);
  void set_page_size(int page_size);
};
std::ostream &operator <<(std::ostream &ostr, const process proc);
#endif
