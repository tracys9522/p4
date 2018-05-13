#ifndef PAGE_H
#define PAGE_H
#include <ostream>
using namespace std;
class page
{
  int _process_id;
  int _page_id;
  int _last_referenced; //timestamp
  int _frequency;
  bool _memory;

public:
  page();
  page(int process_id, int page_id, int _last_referenced, int frequency);

  int get_process_id() const;
  int get_page_id() const;
  int get_last_ref() const;
  int get_frequency() const;
  bool in_memory() const;

  void set_process_id(int process_id);
  void set_page_id(int page_id);
  void set_last_referenced(int last_referenced);
  void set_frequency(int frequency);
  void set_memory(bool memory);
  
  void free_page(int time);
};

void FIFO();
void LFU();
void LRU();
void MFU();
void RAND();

ostream &operator <<(ostream &ostr, const page pg);
#endif
