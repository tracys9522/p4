#include <iostream>
#include <list>
#include <queue>
#include "page.h"
#include "process.h"

using namespace std;

queue <process> job_queue; //150
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
    page newpage = page();
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

    //qsort(job_queue,150,sizeof(process),arrival_compare);
    for (int i = 0; i < 150; i++) {
      cout<<job_queue.front()<<endl;
      job_queue.pop();
    }



  //}
}
