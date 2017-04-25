# Scheduler

最近再看看kafka源码，有个任务调度器挺有意思的，所以自己用C++写了一个很简单的调度器，支持
1. 立即调度
2. 延迟调度
3. 循环调度

代码很简单，我也写了一个简单的案例，代码如下:
```c++
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include"scheduler.h"

void* testNormal(void *arg){
    printf("test normal %ld\n",(long)arg);
}

void* testDelay(void *arg){
    printf("test delay %ld\n",(long)arg);
}

void* testCircle(void *arg){
    printf("testCircle %ld\n",(long)arg);
}

int main(void){
  sched::Scheduler *s=new sched::Scheduler(5);
  int res=s->Startup();
  if(res!=0){
    fprintf(stderr,"Scheduler startup error\n");
    exit(-1);
  }
  int err = s->Schedule(testNormal,(void*)1,0,0);
  if(err!=0){
    fprintf(stderr,"Schedule error\n");
    exit(-1);
  }
  err = s->Schedule(testDelay,(void*)2,2,0);
  if(err!=0){
    fprintf(stderr,"Schedule error\n");
    exit(-1);
  }
  err = s->Schedule(testCircle,(void*)3,0,5);
  if(err!=0){
    fprintf(stderr,"Schedule error\n");
    exit(-1);
  }
  pause();
  return 0;
}
```
可以得到如下结果为:
```
$ ./main
scheduler startup successfully!
test normal 1
testCircle 3
test delay 2
testCircle 3
testCircle 3
...
```

# 原理

原理很简单，和大部分调度器类似，也是生产者和消费者模型。有一个队列保存所有的任务，开启一个线程不断消费队列里的任务，调度器所在线程往队列中添加任务。
