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

// priority_queue<process*, vector<process*>,compare> job_queue; //150
list<page*> page_list;     //100

//process size in pages
int page_size[4] = {5,11,17,31};

//evenly distributed chance
int processSize()
{
  int choice = rand()%4;
  return page_size[choice];
}

//100 pages as linked list
// void initial_page_list()
// {
//   for (int i = 0; i < 100; i++)
//   {
//     int arrive = rand()%60;
//     page* newpage = new page(i,arrive);
//     page_list.push_back(newpage);
//   }
// }

//generate 150 jobs
priority_queue<process*, vector<process*>, compare> generate_job_queue()
{
  #ifdef DEBUG
  fprintf(stderr, "generating jobs queue\n");
  #endif
  priority_queue<process*, vector<process*>, compare> job_queue;
  for (int i = 0; i < kNumJobs; i++)
  {
    int size = processSize();
    int arrival = rand()%kTotalRuntime;
    int service = rand()%5+1;
    // TODO: Generate `service * 10` pages for each process and
    //       place them in an std::queue.
    process* newprocess = new process(i, size, arrival, service);
    job_queue.push(newprocess);
  }
  #ifdef DEBUG
  fprintf(stderr, "generated jobs queue\n");
  #endif
  return job_queue;
}

bool in_list(page *new_page)
{
  bool found = false;
  // fprintf(stderr, "searching for (%d-%d)\n", new_page->get_pid(), new_page->get_addr());
  for (auto it : page_list) {
    if (new_page->get_pid() == it->get_pid() && new_page->get_addr() == it->get_addr()) {
      // return true;
      // fprintf(stderr, "found for (%d-%d)\n", it->get_pid(), it->get_addr());
      found = true;
    } else {
      // fprintf(stderr, "not found for (%d-%d)\n", it->get_pid(), it->get_addr());
    }
  }
  // return false;
  #ifdef DEBUG
  if (found) {
    fprintf(stderr, "RETURN found\n");
  } else {
    fprintf(stderr, "RETURN not found\n");
  }
  #endif
  return found;

  // // list <page>::iterator it;
	// auto it = find(page_list.begin(), page_list.end(),new_page);
	// if (it != page_list.end())
	// {
	// 	return true;
	// }
	// return false;
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
	// for (list<page*>::iterator it = page_list.begin(); it != page_list.end(); ++it)
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

// bool FIFO(page *new_page)
// {
// 	bool hit = false;
//
// 	//page not in list
// 	if (!in_list(new_page))
// 	{
//     fprintf(stderr, "page (%d-%d) not in list(%ld)\n", new_page->get_pid(), new_page->get_addr(), page_list.size());
// 		//page list is full
// 		if (page_list.size() >= kMaxListLen)
// 		{
// 			page_list.pop_front();
// 		}
// 		page_list.push_back(new_page);
// 	}
//
// 	//page in list
// 	else
// 	{
// 		hit = true;
// 	}
// 	return hit;
// }

bool FIFO(page* p) {
  // fprintf(stderr, "checking (%d-%d) against PL(%ld)\n", p->get_pid(), p->get_addr(), page_list.size());
  if (in_list(p)) {
    return true;
  }
  if (page_list.size() >= kMaxListLen) {
    #ifdef DEBUG
    fprintf(stderr, "swapping in (%d-%d) to PL(%ld) -> ", p->get_pid(), p->get_addr(), page_list.size());
    #endif
    page_list.pop_front();
  } else {
    #ifdef DEBUG
    fprintf(stderr, "adding (%d-%d) to PL(%ld) -> ", p->get_pid(), p->get_addr(), page_list.size());
    #endif
  }
  page* pnew = new page(p->get_pid(), p->get_addr(), 0);
  page_list.push_back(pnew);
  #ifdef DEBUG
  fprintf(stderr, "new PL(%ld)\n", page_list.size());
  #endif

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
			// advance(it, random(0, page_list.size()-1));
      advance(it, rand() % page_list.size()); // FIXME: is this correct?
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
    // fprintf(stderr, "[%d] added process %d to plist(%ld)\n", t, p->get_pid(), page_list.size());

    return true;
  }
  // fprintf(stderr, "[%d] process %d hasn't arrived yet (arrives at %d)\n", t/kTicksPerSec, p->get_pid(), p->get_arrival_time());
  return false;
}

results_t simulate(ReplaceFunc replace) {
  auto job_queue = generate_job_queue();
  page_list.clear();

  int hits = 0, misses = 0;

  int brk_count = 0, ave_pages = 0; // TODO: remove

  unordered_map<int, process*> current_processes; // pid as index

  for (int t = 0; t < kTotalRuntime * kTicksPerSec; t++) {  // run for 60 "seconds"
  // for (int t = 0; t < 5; t++) {  // run for 60 "seconds"
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
        #ifdef DEBUG
        fprintf(stderr, "[%d] removing process %d from plist(%ld) and proc_queue(%ld)\n", t, it->second->get_pid(), page_list.size(), current_processes.size());
        #endif
        delete it->second;
        it = current_processes.erase(it);
        add_new_procs(t, current_processes, job_queue);

        ++brk_count;
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
    ave_pages += page_list.size();
  }

  #ifdef DEBUG
  fprintf(stderr, "breaks: %d\n", brk_count);
  fprintf(stderr, "ave pages: %d\n", ave_pages / (kTotalRuntime*kTicksPerSec));
  fprintf(stderr, "pages left: %lu\n", job_queue.size());
  fprintf(stderr, "returning\n");
  #endif
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

  // uncomment for testing
  // test();
  // return 0;

  int hits = 0, misses = 0;
  for (int i = 0; i < 5; i++) {

		// initial_page_list();
		// generate_job_queue();

    // fprintf(stderr, "page list has %ld pages\n", page_list.size());

		// priority queue sorted by arrival time
		// for (int i = 0; i < 150; i++) {
		//   cout<<job_queue.top()<<endl;
		//   job_queue.pop();
		// }

    results_t results = simulate(repl_func);
    #ifdef DEBUG
    fprintf(stderr, "returned\n");
    #endif
    printf("[%d] hits:   %5d / %5d\n", i, results.hits,   results.hits+results.misses);
    printf("[%d] misses: %5d / %5d\n", i, results.misses, results.hits+results.misses);
    hits   += results.hits;
    misses += results.misses;
  }
  printf("total hits:   %5d / %6d\n", hits,   hits+misses);
  printf("total misses: %5d / %6d\n", misses, hits+misses);
}
