#ifndef PAGE_H
#define PAGE_H
#include <ostream>

class page
{
  int _process_id;
  int _addr;
  // int _arrival_time;
  int _last_referenced; //timestamp
  int _frequency;
  bool _memory;
  int _page_size;

public:
  page();
  // page(int page_id, int arrival_time);
  page(int process_id, int addr, int page_size);
  //page(int process_id, int page_id, int _last_referenced, int frequency);

  int get_pid() const;
  int get_addr() const;
  // int get_arrival_time() const;
  int get_last_ref() const;
  int get_frequency() const;
  bool in_memory() const;

  void set_pid(int process_id);
  void set_addr(int addr);
  // void set_arrival_time(int arrival);
  void set_last_ref(int last_referenced);
  void set_frequency(int frequency);
  void set_memory(bool memory);

  void free_page(int time);
};
std::ostream &operator <<(std::ostream &ostr, const page pg);
#endif
