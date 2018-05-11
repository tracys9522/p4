#include <iostream>
#include <list>
#include <queue>

using namespace std;

queue <process*> job_queue[150];
list <page*> page_list[100];

//process size in pages
int page_size[4];
page_size[0] = 5;
page_size[1] = 11;
page_size[2] = 17;
page_size[3] = 31;

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
    newpage.pid = -1;
    page_list.push_back(newpage);
  }
}

//generate 150 jobs
void generate_job_queue()
{
  for(int i = 0; i < 150; i++)
  {
    process newprocess = process();
    newprocess.pid = i;
    newprocess.process_size = processSize();
    newprocess.arrival_time = rand()%60;
    newprocess.service_duration = rand()%150;
    job_queue.push(newprocess);
  }
}

//compare arrival time
int arrival_compare(const void *s1, const void *s2)
{
    process *a = (process *)s1;
    process *b = (process *)s2;
    return a->arrival_time - b->arrival_time;
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
