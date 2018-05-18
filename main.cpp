#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include "page.h"
#include "process.h"
#include "constants.h"

// uncomment this for debug output
// #define DEBUG

using namespace std;

struct compare
{
  bool operator () (const process *a, const process *b){
    return a->get_arrival_time() > b->get_arrival_time();
  }
};

struct results_t {
  int hits;
  int misses;

  results_t(int h, int m) {
    hits   = h;
    misses = m;
  }
};

typedef bool (*ReplaceFunc)(page*);

list<page*> page_list;

// process size in pages
int page_size[4] = {5,11,17,31};

// evenly distributed chance
int processSize()
{
  int choice = rand()%4;
  return page_size[choice];
}

void print_memory_map() {
  cout << "< ";
  for (auto it : page_list) {
    cout << it->get_pid() << " , ";
  }
  cout << ">" << endl;
}

// generate 150 jobs
priority_queue<process*, vector<process*>, compare> generate_job_queue()
{
  priority_queue<process*, vector<process*>, compare> job_queue;
  for (int i = 0; i < kNumJobs; i++)
  {
    int size = processSize();
    int arrival = rand()%kTotalRuntime;
    int service = rand()%5+1;
    process* newprocess = new process(i, size, arrival, service);
    job_queue.push(newprocess);
  }

  return job_queue;
}

bool in_list(page *new_page)
{
  bool found = false;
  for (auto it : page_list) {
    if (new_page->get_pid() == it->get_pid() && new_page->get_addr() == it->get_addr()) {
      found = true;
    }
  }
  return found;
}

void remove_min()
{
	int min = (*page_list.begin())->get_last_ref();
	for (auto it = page_list.begin(); it != page_list.end(); ++it)
	{
		if ((*it)->get_last_ref() <= min)
		{
			min = (*it)->get_last_ref();
		}
	}
	for (auto it = page_list.begin(); it != page_list.end(); ++it)
	{
		if ((*it)->get_last_ref() == min)
		{
			page_list.erase(it);
			break;
		}
	}
	return;
}

void remove_max()
{
	int max = (*page_list.begin())->get_last_ref();
	for (auto it = page_list.begin(); it != page_list.end(); ++it)
	{
		if ((*it)->get_last_ref() >= max)
		{
			max = (*it)->get_last_ref();
		}
	}
  for (auto it = page_list.begin(); it != page_list.end(); it++)
	{
		if ((*it)->get_last_ref() == max)
		{
			page_list.erase(it);
			break;
		}
	}
	return;
}

bool FIFO(page* p) {
  if (in_list(p)) {
    return true;
  }
  if (page_list.size() >= kMaxListLen) {
    page_list.pop_front();
  }
  page* pnew = new page(p->get_pid(), p->get_addr(), 0);
  page_list.push_back(pnew);

  return false;
}

bool LRU(page *new_page)
{
	bool hit = false;

	//page not in list
	if (!in_list(new_page))
	{
		//page list is full
		if (page_list.size() >= kMaxListLen)
		{
			//front is least recently used
			page_list.pop_front();
		}
		page_list.push_back(new_page);
	}

	//page in list
	else
	{
		hit = true;
		page_list.remove(new_page);
		page_list.push_back(new_page);
	}
	return hit;
}

bool LFU(page *new_page)
{
	bool hit = false;

	//page not in list
	if (!in_list(new_page))
	{
		//page list is full
		if (page_list.size() >= kMaxListLen)
		{
			remove_min();
		}
		new_page->set_last_ref(0);
		page_list.push_back(new_page);
	}

	//page in list
	else
	{
		hit = true;
		int temp = new_page->get_last_ref();
		page_list.remove(new_page);
		new_page->set_last_ref(temp + 1);
		page_list.push_back(new_page);
	}
	return hit;
}

bool MFU(page *new_page)
{
	bool hit = false;

	//page not in list
	if (!in_list(new_page))
	{
		//page list is full
		if (page_list.size() >= kMaxListLen)
		{
			remove_max();
		}
		new_page->set_last_ref(0);
		page_list.push_back(new_page);
	}

	//page in list
	else
	{
		hit = true;
		int temp = new_page->get_last_ref();
		page_list.remove(new_page);
		new_page->set_last_ref(temp + 1);
		page_list.push_back(new_page);
	}
	return hit;
}

bool RAND(page *new_page)
{
	bool hit = false;

	//page not in list
	if (!in_list(new_page))
	{
		//page list is full
		if (page_list.size() >= kMaxListLen)
		{
			list<page*>::iterator it = page_list.begin();
      advance(it, rand() % page_list.size());
			page_list.erase(it);
		}
		page_list.push_back(new_page);
	}

	//page in list
	else
	{
		hit = true;
	}
	return hit;
}

int remove_process_pages(int pid) {
  int count = 0;
  for (auto it = page_list.begin(); it != page_list.end(); /* nothing */) {
    if ((*it)->get_pid() == pid) {
      it = page_list.erase(it);
      ++count;
    } else {
      ++it;
    }
  }
  #ifdef DEBUG
  fprintf(stderr, "Removed %d pages with pid %d\n", count, pid);
  #endif
  return count;
}

bool add_new_procs(int t, unordered_map<int, process*>& current_processes, priority_queue<process*, vector<process*>, compare>& job_queue) {
  process* p = job_queue.top();
  if (p->get_arrival_time() <= (t / kTicksPerSec) &&
      page_list.size() + 4 <= kMaxListLen) {
    // Start a new process iff a new process has arrived by this time and
    // there's at least four free pages in the free page list.
    current_processes.insert(pair<int, process*>(p->get_pid(), p));
    job_queue.pop();

    printf("[%02d.%02d] PID %d swapped in (%d pages for %ds)\n", t/kTicksPerSec, t%kTicksPerSec, p->get_pid(), p->get_page_size(), p->get_running_time());
    print_memory_map();
    return true;
  }

  return false;
}

results_t simulate(ReplaceFunc replace) {
  auto job_queue = generate_job_queue();
  page_list.clear();

  int hits = 0, misses = 0;

  unordered_map<int, process*> current_processes; // pid as index

  for (int t = 0; t < kTotalRuntime * kTicksPerSec; t++) {  // run for 60 "seconds"
    if (job_queue.size() == 0) {
      break;
    }
    add_new_procs(t, current_processes, job_queue);

    for (auto it = current_processes.begin(); it != current_processes.end(); /* nothing */ ) {
      auto pages = it->second->get_pages();
      if (pages->size() == 0) {
        // if process has no more memory references, then:
        // - remove all of its pages from the page table
        remove_process_pages(it->second->get_pid());
        // - remove it from the process map

        printf("[%02d.%02d] PID %d swapped out (%d pages for %ds)\n", t/kTicksPerSec, t%kTicksPerSec, it->second->get_pid(), it->second->get_page_size(), it->second->get_running_time());
        print_memory_map();

        #ifdef DEBUG
        fprintf(stderr, "[%d] removing process %d from plist(%ld) and proc_queue(%ld)\n", t, it->second->get_pid(), page_list.size(), current_processes.size());
        #endif
        delete it->second;
        it = current_processes.erase(it);
        add_new_procs(t, current_processes, job_queue);

        break;
      }

      // do the next memory reference (& increment hits/misses)
      bool hit = replace(&(pages->front()));
      if (hit) {
        ++hits;
      } else {
        ++misses;
      }

      // pop the memory reference off the process's queue
      #ifdef DEBUG
      fprintf(stderr, "[%d] accessed & removed page (%d-%d) from proc %d, %lu -> ", t, pages->front().get_pid(), pages->front().get_addr(), it->second->get_pid(), pages->size());
      #endif
      pages->pop();
      #ifdef DEBUG
      fprintf(stderr, "%lu left\n", pages->size());
      #endif

      ++it;
    }
  }

  return results_t(hits, misses);
}

void test() {
  auto job_queue = generate_job_queue();

  // this is quite destructive
  while (job_queue.size() > 0) {
    std::cout << job_queue.top()->str() << std::endl;
    job_queue.pop();
  }

  page* p = new page(10, 6, 0);
  FIFO(p);
  p = new page(127, 8, 0);
  FIFO(p);
  printf("size of page list: %lu\n", page_list.size());
}

int main()
{
  ReplaceFunc repl_func;

  int input;
  cout << "please input which paging algo to use..." << endl;
  cout << "1) FIFO" << endl;
  cout << "2) LRU" << endl;
  cout << "3) LFU" << endl;
  cout << "4) MFU" << endl;
  cout << "5) Random" << endl;

  cin >> input;

  switch (input) {
    case 1:
      repl_func = FIFO;
      break;
    case 2:
      repl_func = LRU;
      break;
    case 3:
      repl_func = LFU;
      break;
    case 4:
      repl_func = MFU;
      break;
    case 5:
      repl_func = RAND;
      break;
    default:
      cout << "error" <<endl;
      return 0;
  }

  // srand(time(0));

  int hits = 0, misses = 0;
  for (int i = 0; i < 5; i++) {
    results_t results = simulate(repl_func);

    printf("[%d] hits:   %5d / %5d\n", i, results.hits,   results.hits+results.misses);
    printf("[%d] misses: %5d / %5d\n", i, results.misses, results.hits+results.misses);
    hits   += results.hits;
    misses += results.misses;
  }
  printf("total hits:   %5d / %6d = %f\n", hits,   hits+misses, hits   / (float)(hits+misses));
  printf("total misses: %5d / %6d = %f\n", misses, hits+misses, misses / (float)(hits+misses));
}
