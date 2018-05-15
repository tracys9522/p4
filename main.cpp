#include <iostream>
#include <list>
#include <queue>
#include <algorithm>
#include "page.h"
#include "process.h"

using namespace std;

struct compare
{
  bool operator () (const process &a, const process &b){
    return a.get_arrival_time() > b.get_arrival_time();
  }
};

priority_queue <process,vector<process>,compare> job_queue; //150
list <page> page_list;     //100

//process size in pages
int page_size[4] = {5,11,17,31};

//evenly distributed chance
int processSize()
{
  int choice = rand()%4;
  return page_size[choice];
}

//100 pages as linked list
void initial_page_list()
{
  for(int i = 0; i < 100; i++)
  {
    int arrive = rand()%60;
    page newpage = page(i,arrive);
    page_list.push_back(newpage);
  }
}

//generate 150 jobs
bool generate_job_queue()
{
  for(int i = 0; i < 150; i++)
  {
    int size = processSize();
    int arrival = rand()%60;
    int service = rand()%150;
    process newprocess = process(i, size, arrival, service);
    job_queue.push(newprocess);
  }
}

bool in_list(page *new_page)
{
	list <page>::iterator it;
	it = find(page_list.begin(), page_list.end(),new_page);
	if(it != page_list.end())
	{
		return true;
	}
	return false;
}

void remove_min()
{
	int min = page_list.begin()->_last_referenced;
	for (list<page>::iterator it = page_list.begin(); it != page_list.end(); ++it)
	{
		if(it->_last_referenced <= min)
		{
			min = it->_last_referenced;
		}
	}
	for (list<page>::iterator it = page_list.begin(); it != page_list.end(); ++it)
	{
		if(it->_last_referenced == min)
		{
			page_list.erase(it);
			break;
		}
	}
	return;
}

void remove_max()
{
	int max = page_list.begin()->_last_referenced;
	for (list<page>::iterator it = page_list.begin(); it != page_list.end(); ++it)
	{
		if(it->_last_referenced >= max)
		{
			max = it->_last_referenced;
		}
	}
	for (list<page>::iterator it = page_list.begin(); it != page_list.end(); ++it)
	{
		if(it->_last_referenced == min)
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
	if(!in_list(new_page))
	{
		//page list is full
		if(page_list.size() >= 50)
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
	if(!in_list(new_page))
	{
		//page list is full
		if(page_list.size() >= 50)
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
	if(!in_list(new_page))
	{
		//page list is full
		if(page_list.size() >= 50)
		{
			remove_min();
		}
		new_page->_last_referenced = 0;
		page_list.push_back(new_page);
	}
	
	//page in list
	else
	{
		hit = true;
		int temp = new_page->_last_referenced;
		page_list.remove(new_page);
		new_page->_last_referenced = temp + 1;
		page_list.push_back(new_page);
	}
	return hit;
}

bool MFU(page *new_page)
{
	bool hit = false;
	
	//page not in list
	if(!in_list(new_page))
	{
		//page list is full
		if(page_list.size() >= 50)
		{
			remove_max();
		}
		new_page->_last_referenced = 0;
		page_list.push_back(new_page);
	}
	
	//page in list
	else
	{
		hit = true;
		int temp = new_page->_last_referenced;
		page_list.remove(new_page);
		new_page->_last_referenced = temp + 1;
		page_list.push_back(new_page);
	}
	return hit;
}

bool RAND(page *new_page)
{
	bool hit = false;
	
	//page not in list
	if(!in_list(new_page))
	{
		//page list is full
		if(page_list.size() >= 50)
		{
			list<page>::iterator it = page_list.begin();
			advance(it,random(0,page_list.size()-1));
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

int main()
{
  /*
  int input;
  cout << "please input which paging algo to use..." << endl;
  cout << "1) FIFO" << endl;
  cout << "2) LRU" << endl;
  cout << "3) LFU" << endl;
  cout << "4) MFU" << endl;
  cout << "5) Random" << endl;

  cin >> input;

  struct data d;
  int seed = time(0);
  srand(seed);

  //for (int i = 0; i < 5; i++) {

		initial_page_list();
		generate_job_queue();
		
		
		//if new page
		bool hit = false;
		switch (input) {
			case 1:
			  hit = FIFO(*new_page);
			  break;
			case 2:
			  hit = LRU(*new_page);
			  break;
			case 3:
			  hit = LFU(*new_page);
			  break;
			case 4:
			  hit = MFU(*new_page);
			  break;
			case 5:
			  hit = RAND(*new_page);
			  break;
			default:
			  cout << "error" <<endl;
			  return 0;
		}
	
		//priority queue sorted by arrival time
		for (int i = 0; i < 150; i++) {
		  cout<<job_queue.top()<<endl;
		  job_queue.pop();
		}

  //}
}
