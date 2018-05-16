#include "page.h"

page::page()
{
  _process_id = -1;
  _addr = -1;
  // _arrival_time = -1;
  _last_referenced = -1;
  _frequency = 0;
  _memory = false;
  _page_size = 11;
}
// page::page(int page_id, int arrival)
// {
//   _page_id = page_id;
//   _arrival_time = arrival;
//
//   _process_id = -1;
//   _last_referenced = -1;
//   _frequency = 0;
//   _memory = false;
// }
page::page(int process_id, int addr, int page_size)
{
  _process_id = process_id;
  _addr = addr;
  // _arrival_time = arrival;

  _last_referenced = -1;
  _frequency = 0;
  _memory = false;
  _page_size = page_size;
}
/*
page::page(int process_id, int page_id, int last_referenced, int frequency)
{
  _process_id = process_id;
  _page_id = page_id;
  _last_referenced = last_referenced; //timestamp
  _frequency = frequency;
}
*/
int page::get_pid()const{ return _process_id; }
int page::get_addr()const{ return _addr; }
// int page::get_arrival_time()const {return _arrival_time; }
int page::get_last_ref()const{ return _last_referenced; }
int page::get_frequency()const{ return _frequency; }
bool page::in_memory()const{ return _memory == 0? false : true; }

void page::set_pid(int process_id){ _process_id = process_id; }
void page::set_addr(int addr){ _addr = addr; }
void page::set_last_ref(int last_referenced){ _last_referenced = last_referenced; }
void page::set_frequency(int frequency){ _frequency = frequency; }
void page::set_memory(bool memory){ _memory = memory; }
void page::free_page(int time){
    _memory = 0;
    _last_referenced = time;
}

std::ostream &operator <<(std::ostream &ostr, const page pg)
{
    ostr << "page pid: " << pg.get_pid() << ", page id: " << pg.get_addr();
    return ostr;
}
