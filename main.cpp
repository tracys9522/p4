#include <iostream>
#include <list>
#include <queue>
#include "page.h"
#include "process.h"

using namespace std;

queue <process*> job_queue[150];
list <page*> page_list[100];

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
    page newpage = page();
    newpage._pid = -1;
    page_list.push_back(newpage);
  }
}

//generate 150 jobs
void generate_job_queue()
{
  for(int i = 0; i < 150; i++)
  {
    process newprocess = process();
    newprocess._pid = i;
    newprocess._process_size = processSize();
    newprocess._arrival_time = rand()%60;
    newprocess._service_duration = rand()%150;
    job_queue.push(newprocess);
  }
}

//compare arrival time
int arrival_compare(const void *s1, const void *s2)
{
    process *a = (process *)s1;
    process *b = (process *)s2;
    return a->_arrival_time - b->_arrival_time;
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

  switch (input) {
    case 1:
      FIFO();
      break;
    case 2:
      LRU();
      break;
    case 3:
      LFU();
      break;
    case 4:
      MFU();
      break;
    case 5:
      RAND();
      break;
    default:
      cout << "error" <<endl;
      return 0;
  }
  */

  int page_hit;
  int page_miss;

  int seed = time(0);
  srand(seed);

  //for (int i = 0; i < 5; i++)
  //{
    page_hit = 0;
    page_miss = 0;

    initial_page_list();
    generate_job_queue();
    qsort(job_queue,150,sizeof(process),arrival_compare);

  //}
}
