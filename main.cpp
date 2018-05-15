#include <iostream>
#include <list>
#include <queue>
#include "page.h"
#include "process.h"

using namespace std;

struct compare
{
  bool operator () (const process &a, const process &b){
    return a.get_arrival_time() > b.get_arrival_time();
  }
};

struct data
{
	int hit;
	int miss;
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
void generate_job_queue()
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

void FIFO(struct *data)
{
	/*
	if page is not in table
		data->miss++;
			
		if table is full
			page_list.pop_front();
			
		page_list.push_back(page);
		
	*/	
}

void LRU(struct *data)
{
	/*
	if page is not in table
		data->miss++;
			
		if table is full
			page_list.pop_front();
		
		page_list.push_back(page);
		
	if page is in table 
		page_list.push_back(page);
	*/	
}

void LFU(struct *data)
{
	/*
	if page is not in table
		data->miss++;
			
		if table is full
			remove least referenced
		
		page_list.push_back(page);
		set new page reference to 0
		set least referenced
		
	if page is in table 
		move page to back and increment reference
		set least referenced
	*/
	
}

void MFU(struct *data)
{
	/*
	if page is not in table
		data->miss++;
			
		if table is full
			remove most referenced
		
		page_list.push_back(page);
		set new page reference to 0
		set most referenced
		
	if page is in table 
		move page to back and increment reference	
		set most referenced
	*/
}

void RAND(struct *data)
{
	/*
	if page is not in table
		data->miss++;
			
		if table is full
			remove random
		
		page_list.push_back(page);
		
	*/
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
		d.hit = 0;
		d.miss = 0;

		initial_page_list();
		generate_job_queue();
		
		
		//simulation
		
		switch (input) {
			case 1:
			  FIFO(*d);
			  break;
			case 2:
			  LRU(*d);
			  break;
			case 3:
			  LFU(*d);
			  break;
			case 4:
			  MFU(*d);
			  break;
			case 5:
			  RAND(*d);
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
