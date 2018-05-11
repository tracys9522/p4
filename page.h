#ifndef PAGE_H
#define PAGE_H
#include <ostream>
class page
{
  int _pid;
  int _page_id;
  int _page_referenced;
  int _page_num;
  int _last_accessed; //timestamp
  enum {NOT_IN_MEM = 0, IN_MEM} _state;

public:
  page();
  page(int pid, int page_id, int page_refer, int page_num, int last_accessed);
  
  set_state();
  bool in_mem() const;
};
ostream &operator <<(ostream &ostr, const page pg);
#endif