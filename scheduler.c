#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define n 4

 int j,timer,remain,flag=0,timer_quantum,turn=0; 
  int wait_timer=0,turnaround_timer=0,at[10],bt[10],rt[10]; 
void StartNext(int tid)     //choose the next thread to run
{
    int i;
    for(i = (tid + 1) % n; bt[i] == 0; i = (i + 1) % n)
        if(i == tid)    //if every thread has finished
            return;
    turn = i;
}

void *Run(void *tid)    //the thread function
{
    int i = (int)tid;
    for(timer=0,i=0;remain!=0;) 
  { 
    if(rt[i]<=timer_quantum && rt[i]>0) 
    { 
      timer+=rt[i]; 
      rt[i]=0; 
      flag=1; 
    } 
    else if(rt[i]>0) 
    { 
      rt[i]-=timer_quantum; 
      timer+=timer_quantum; 
    } 
    if(rt[i]==0 && flag==1) 
    { 
      remain--; 

      wait_timer+=timer-at[i]-bt[i]; 
      turnaround_timer+=timer-at[i]; 
      flag=0; 
    } 
    if(i==n-1) 
      i=0; 
    else if(at[i+1]<=timer) 
      i++; 
    else 
      i=0; 
   
        StartNext(i);   //choose the next thread to run
    }
 pthread_exit(0);
}

int main()
{
    pthread_t threads[n];
    int i, status;

   remain=n; 
  for(i=0;i<n;i++) 
  { 
    printf("Enter Arrival timer and Burst timer for Process Process Number %d :",i+1); 
    scanf("%d",&at[i]); 
    scanf("%d",&bt[i]); 
    rt[i]=bt[i]; 
  } 
  printf("Enter timer Quantum:\t"); 
  scanf("%d",&timer_quantum); 
        for(i = 0; i < n; i++)
        {
            status = pthread_create(&threads[i], NULL, Run, (void *)i);    //Create threads
            if(status != 0)
            {
                printf("While creating thread %d, pthread_create returned error code %d\n", i, status);
                exit(-1);
            }
            pthread_join(threads[i], 0);    //Join threads
        }

     printf("\nTotal Waiting timer= %f\n",wait_timer*1.0);
    return 0;
}
