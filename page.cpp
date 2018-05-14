#include "page.h"

page::page()
{
  _process_id = -1;
  _page_id = -1;
  _arrival_time = -1;
  _last_referenced = -1;
  _frequency = 0;
  _memory = false;
}
page::page(int page_id, int arrival)
{
  _page_id = page_id;
  _arrival_time = arrival;

  _process_id = -1;
  _last_referenced = -1;
  _frequency = 0;
  _memory = false;
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
int page::get_process_id()const{ return _process_id; }
int page::get_page_id()const{ return _page_id; }
int page::get_last_ref()const{ return _last_referenced; }
int page::get_frequency()const{ return _frequency; }
bool page::in_memory()const{ return _memory == 0? false : true; }

void page::set_process_id(int process_id){ _process_id = process_id; }
void page::set_page_id(int page_id){ _page_id = page_id; }
void page::set_last_referenced(int last_referenced){ _last_referenced = last_referenced; }
void page::set_frequency(int frequency){ _frequency = frequency; }
void page::set_memory(bool memory){ _memory = memory; }

void page::free_page(int time){
    _memory = 0;
    _last_referenced = time;
}

std::ostream &operator <<(std::ostream &ostr, const page pg)
{
    ostr << "page pid: " << pg.get_process_id() << ", page id: " << pg.get_page_id();
    return ostr;
}
