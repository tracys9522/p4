#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include "page.h"
#include "process.h"
#include "constants.h"

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

priority_queue<process*, vector<process*>,compare> job_queue; //150
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
bool generate_job_queue()
{
  for (int i = 0; i < 150; i++)
  {
    int size = processSize();
    int arrival = rand()%60;
    int service = rand()%150;
    // TODO: Generate `service * 10` pages for each process and
    //       place them in an std::queue.
    process* newprocess = new process(i, size, arrival, service);
    job_queue.push(newprocess);
  }
}

bool in_list(page *new_page)
{
	// list <page>::iterator it;
	auto it = find(page_list.begin(), page_list.end(),new_page);
	if (it != page_list.end())
	{
		return true;
	}
	return false;
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

bool FIFO(page *new_page)
{
	bool hit = false;

	//page not in list
	if (!in_list(new_page))
	{
		//page list is full
		if (page_list.size() >= 50)
		{
			page_list.pop_front();
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

bool LRU(page *new_page)
{
	bool hit = false;

	//page not in list
	if (!in_list(new_page))
	{
		//page list is full
		if (page_list.size() >= 50)
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
		if (page_list.size() >= 50)
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
		if (page_list.size() >= 50)
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
		if (page_list.size() >= 50)
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
    if ((*it)->get_process_id() == pid) {
      it = page_list.erase(it);
      ++count;
    } else {
      ++it;
    }
  }
  fprintf(stderr, "Removed %d pages with pid %d\n", count, pid);
  return count;
}

results_t simulate(ReplaceFunc replace) {
  int hits = 0, misses = 0;

  unordered_map<int, process*> current_processes; // pid as index

  for (int t = 0; t < 60 * kTicksPerSec; t++) {  // run for 60 "seconds"
    process* p = job_queue.top();
    if (p->get_arrival_time() < t / kTicksPerSec &&
        page_list.size() + 4 <= kMaxListLen) {
      // Start a new process iff a new process has arrived by this time and
      // there's at least four free pages in the free page list.
      current_processes.insert(pair<int, process*>(p->get_pid(), p));
      job_queue.pop();
    }

    for (auto it = current_processes.begin(); it != current_processes.end(); /* nothing */ ) {
      auto pages = it->second->get_pages();
      if (pages.size() == 0) {
        // if process has no more memory references, then:
        // - remove all of its pages from the page table
        remove_process_pages(it->second->get_pid());
        // - remove it from the process map
        it = current_processes.erase(it);

        break;
      }

      // do the next memory reference (& increment hits/misses)
      bool hit = replace(&pages.front());
      if (hit) {
        ++hits;
      } else {
        ++misses;
      }

      // pop the memory reference off the process's queue
      pages.pop();

      ++it;
    }
  }

  return results_t(hits, misses);
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

  int seed = time(0);
  srand(seed);

  for (int i = 0; i < 5; i++) {

		// initial_page_list();
		generate_job_queue();

		// priority queue sorted by arrival time
		// for (int i = 0; i < 150; i++) {
		//   cout<<job_queue.top()<<endl;
		//   job_queue.pop();
		// }

    results_t results = simulate(repl_func);
  }
}
