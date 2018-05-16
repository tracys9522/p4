#include <sstream>
#include "process.h"
#include "constants.h"

int generate_next_page(int prev) {
  if (prev < 0 || prev > 10) {
    return rand() % 11;
  }
  int next;
  if (rand()%10 < 7) {
    // make it adjacent to prev
    next = prev + rand()%3 - 1 + 11;
  } else {
    next = prev + rand()%8 + 2 + 11;
  }
  return next % 11; // wraps from 0 to 10
}

process::process()
{
    _pid = -1;
    // _page_num = -1;
    _arrival_time = -1;
    _running_time = -1;
    _last_page_used = -1;
    _page_size = -1;
}

process::process(int pid, int page_size, int arrival_time, int running_time)
{
    _pid = pid;
    _arrival_time = arrival_time;
    _running_time = running_time;
    _page_size = page_size;

    // _page_num = -1;
    int pagenum = -1;
    _pages = std::queue<page>();
    for (int i = 0; i < running_time * kTicksPerSec; i++) {
      pagenum = generate_next_page(pagenum);
      _pages.push(page(_pid, pagenum, page_size));
    }
    _last_page_used = -1;
}

int process::get_pid() const { return _pid; }
// int process::get_page_num() const { return _page_num; }
std::queue<page>& process::get_pages() { return _pages; }
int process::get_arrival_time() const { return _arrival_time; }
int process::get_running_time() const { return _running_time; }
int process::get_end_time() const { return _arrival_time + _running_time; }
int process::get_last_page_used() const { return _last_page_used; }
int process::get_page_size() const { return _page_size; }
std::string process::str() {
  std::ostringstream ss;
  ss << "pid: " << _pid << ", psize: " << _page_size << ", arrival: " << _arrival_time << ", runtime: " << _running_time;
  ss << ", pages(" << _pages.size() << "): [ ";
  for (int i = 0; i < _pages.size(); i++) {
    page p = _pages.front();
    ss << p.get_page_id() << " ";
    _pages.push(p);
    _pages.pop();
  }
  ss << "]";
  return ss.str();
}

void process::set_pid(int pid) { _pid = pid; }
// void process::set_page_num(int page_num) { _page_num = page_num; }
void process::set_arrival_time(int arrival_time) { _arrival_time = arrival_time; }
void process::set_last_page_used(int page_used) { _last_page_used = page_used; }
void process::set_page_size(int page_size) { _page_size = page_size; }

std::ostream &operator <<(std::ostream &ostr, const process proc)
{
    ostr <<"process pid: " << proc.get_pid();
    ostr<< "  page size: " << proc.get_page_size();
    ostr<< "  arrival: " << proc.get_arrival_time();
    ostr<< "  service duration: " << proc.get_running_time();
    return ostr;
}
