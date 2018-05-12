#ifndef PAGE_H
#define PAGE_H
#include <ostream>
using namespace std;
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
  
  int get_pid()const;
  int get_page_id()const;
  int get_page_ref()const;
  int get_time()const;
    
  void set_refer(int page_ref);
  void free_page(int time);
  bool in_mem() const;
};

void FIFO();
void LFU();
void LRU();
void MFU();
void RAND();

ostream &operator <<(ostream &ostr, const page pg);
#endif
