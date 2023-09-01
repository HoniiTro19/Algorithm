#include <climits>
#include <cstdio>
#include <cstring>
#include <memory>
#include <pthread.h>
#include <unistd.h>

using namespace std;

typedef void (*task_func_t)(void *args);

#define container_of(ptr, type, member)                                        \
  ({                                                                           \
    const typeof(((type *)0)->member) *__mptr = (ptr);                         \
    (type *)((char *)__mptr - offsetof(type, member));                         \
  })
#define list_entry(ptr, type, member) container_of(ptr, type, member)

struct list_head {
  list_head *prev;
  list_head *next;
};

static inline void init_list_head(list_head *head) {
  head->next = head;
  head->prev = head;
}

static inline int list_empty(list_head *head) { return head->next == head; }

static inline void __list_del(list_head *prev, list_head *next) {
  next->prev = prev;
  prev->next = next;
}

static inline void list_del(list_head *entry) {
  __list_del(entry->prev, entry->next);
}

static inline void __list_add(list_head *newn, list_head *prev,
                              list_head *next) {
  next->prev = newn;
  newn->next = next;
  newn->prev = prev;
  prev->next = newn;
}

static inline void list_add(list_head *newn, list_head *head) {
  __list_add(newn, head, head->next);
}

static inline void list_add_tail(list_head *newn, list_head *head) {
  __list_add(newn, head->prev, head);
}

struct task_t {
  void *args;
  task_func_t func;
  list_head node;
};

struct threadpool_t {
  list_head hlist;
  int thread_num;
  int max_ts_num;
  volatile int curr_ts_num;
  volatile int is_exit;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  pthread_t *ths;
};

static void *_process_task_thread(void *args) {
  threadpool_t *tp = (threadpool_t *)args;
  list_head *pos = NULL;
  task_t *task = NULL;
  if (!args)
    return NULL;
  while (true) {
    pthread_mutex_lock(&tp->mutex);
    while (list_empty(&tp->hlist) && !tp->is_exit) {
      pthread_cond_wait(&tp->cond, &tp->mutex);
    }
    if (tp->is_exit) {
      pthread_mutex_unlock(&tp->mutex);
      break;
    }
    pos = tp->hlist.next;
    list_del(pos);
    --tp->curr_ts_num;
    pthread_mutex_unlock(&tp->mutex);

    task = list_entry(pos, task_t, node);
    task->func(task->args);
    free(task);
  }
  return NULL;
}

threadpool_t *create_threadpool(int thread_nums, int max_ts_num) {
  if (thread_nums <= 0)
    return NULL;
  threadpool_t *tp = (threadpool_t *)malloc(sizeof(threadpool_t));
  if (!tp)
    return NULL;
  memset(tp, 0, sizeof(threadpool_t));
  init_list_head(&tp->hlist);
  tp->is_exit = 0;
  tp->curr_ts_num = 0;
  tp->thread_num = thread_nums;
  tp->max_ts_num = max_ts_num;
  tp->ths = (pthread_t *)malloc(sizeof(pthread_t) * thread_nums);
  pthread_mutex_init(&tp->mutex, NULL);
  pthread_cond_init(&tp->cond, NULL);
  for (int i = 0; i < thread_nums; ++i) {
    pthread_create(&(tp->ths[i]), NULL, _process_task_thread, tp);
  }
  return tp;
}

int add_task_threadpool(threadpool_t *tp, task_func_t func, void *args,
                        int priority) {
  if (!tp)
    return -1;
  if (!func)
    return -2;
  if (tp->curr_ts_num > tp->max_ts_num)
    return -3;
  task_t *task = (task_t *)malloc(sizeof(task_t));
  task->func = func;
  task->args = args;
  pthread_mutex_lock(&tp->mutex);
  if (priority == 1) {
    list_add(&task->node, &tp->hlist);
  } else {
    list_add_tail(&task->node, &tp->hlist);
  }
  ++tp->curr_ts_num;
  pthread_mutex_unlock(&tp->mutex);
  pthread_cond_signal(&tp->cond);
  return 0;
}

int get_ts_num_threadpool(threadpool_t *tp) {
  return tp ? tp->curr_ts_num : -1;
}

int destroy_threadpool(threadpool_t *tp) {
  if (!tp)
    return -1;
  while (!list_empty(&tp->hlist)) {
    continue;
  }
  tp->is_exit = 1;
  pthread_cond_broadcast(&tp->cond);
  for (int i = 0; i < tp->thread_num; ++i) {
    pthread_join(tp->ths[i], NULL);
  }
  pthread_mutex_destroy(&tp->mutex);
  pthread_cond_destroy(&tp->cond);
  free(tp->ths);
  free(tp);
  tp = NULL;
  return 0;
}

const int MAX_THREADPOOL_NUMS = 1000;

typedef struct manage_thpool_t {
  int thpool_nums;
  threadpool_t *thpools[MAX_THREADPOOL_NUMS];
} manage_thpool_t;

manage_thpool_t *create_group_threadpool(int tp_nums, int thread_num,
                                         int max_ts_n) {
  manage_thpool_t *mtp = (manage_thpool_t *)malloc(sizeof(manage_thpool_t));
  if (!mtp)
    return NULL;
  memset(mtp, 0, sizeof(manage_thpool_t));
  mtp->thpool_nums = tp_nums;
  for (int i = 0; i < tp_nums; ++i) {
    mtp->thpools[i] = create_threadpool(thread_num, max_ts_n);
  }
  return mtp;
}

int add_task_group_threadpool(manage_thpool_t *mtp, task_func_t func,
                              void *args, int priority) {
  int ts_num = INT_MAX;
  threadpool_t *tp = NULL;
  for (register int i = 0; i < mtp->thpool_nums; ++i) {
    if (mtp->thpools[i]->curr_ts_num < ts_num) {
      ts_num = mtp->thpools[i]->curr_ts_num;
      tp = mtp->thpools[i];
    }
  }
  if (!tp) {
    tp = mtp->thpools[0];
  }
  return add_task_threadpool(tp, func, args, priority);
}

void destroy_group_threadpool(manage_thpool_t *mtp) {
  if (!mtp)
    return;
  for (int i = 0; i < mtp->thpool_nums; ++i) {
    if (mtp->thpools[i]) {
      destroy_threadpool(mtp->thpools[i]);
    }
  }
}

struct info_t {
  int times;
  char buffer[32];
};

void task1(void *args) {
  info_t *info = (info_t *)args;
  printf("handle task1 pid=%lu times=%d buffer=%s\n", pthread_self(),
         info->times, info->buffer);
  free(args);
}

void task2(void *args) {
  info_t *info = (info_t *)args;
  printf("handle task2 pid=%lu times=%d buffer=%s\n", pthread_self(),
         info->times, info->buffer);
  free(args);
}

void task3(void *args) {
  info_t *info = (info_t *)args;
  printf("handle task3 pid=%lu times=%d buffer=%s\n", pthread_self(),
         info->times, info->buffer);
  free(args);
}

void test_threadpool(void) {
  threadpool_t *tp = create_threadpool(4, 128);
  info_t *info;
  for (int t = 0; t < 10; ++t) {
    for (int i = 0; i < 32; ++i) {
      info = (info_t *)malloc(sizeof(info_t));
      info->times = i;
      sprintf(info->buffer, "Test ThreadPool task1 info...");
      add_task_threadpool(tp, task1, info, 1);

      info = (info_t *)malloc(sizeof(info_t));
      info->times = i;
      sprintf(info->buffer, "Test ThreadPool task2 info...");
      add_task_threadpool(tp, task2, info, 0);

      info = (info_t *)malloc(sizeof(info_t));
      info->times = i;
      sprintf(info->buffer, "Test ThreadPool task3 info...");
      add_task_threadpool(tp, task2, info, 0);
    }
    sleep(1);
  }
  destroy_threadpool(tp);
  printf("Test ThreadPool Fininsh...\n");
}

int main() {
  test_threadpool();
  return 0;
}