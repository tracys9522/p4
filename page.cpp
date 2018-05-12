#include "page.h"

page::page()
{
    _pid = -1;
    _page_id = -1;
    _page_referenced = -1;
    _page_num = -1;
    _last_accessed = -1;
}

page::page(int pid, int page_id, int page_refer, int page_num, int last_accessed)
{
    _pid = pid;
    _page_id = page_id;
    _page_referenced = page_refer;
    _page_num = page_num;
    _last_accessed = last_accessed;
}

int page::get_pid()const{ return _pid; }
int page::get_page_id()const{ return _page_id; }
int page::get_page_ref()const{ return _page_referenced; }
int page::get_time()const { return _last_accessed; }

void page::set_refer(int page_ref){
    _page_referenced = page_ref;
    _state = IN_MEM;
}

void page::free_page(int time){
    _state = NOT_IN_MEM;
    _page_referenced = -1;
    _last_accessed = time;
}

bool page::in_mem() const
{
    return _state == NOT_IN_MEM ? false : true;
}

ostream &operator <<(ostream &ostr, const page pg)
{
    ostr << "page pid: " << pg.get_pid() << ", page id: " << pg.get_page_id();
    return ostr;
}
