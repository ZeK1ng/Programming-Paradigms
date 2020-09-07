#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NEW_WAFFLE_READY "/a"
#define WAFFLE_CHECKED "/b"
#define MANAGER_LOCK "/c"
#define CLIENT_WAFFLE_READY "/d-%d"
#define CLIENT_DONE "/e"
#define CASHIER_LINE_LOCK "/f"
#define CLIENT_WANTS_TO_PAY "/g"
#define READY_FOR_CLIENT "/h-%d"

#define NUM_CLIENTS 10
#define MAX_WAFFLES_PER_CLIENT 4

typedef struct {
  bool approved;
  sem_t* new_waffle_ready;
  sem_t* waffle_checked;
  sem_t* lock;
} ManagerState;

ManagerState manager;

typedef struct {
  int per_client_waffles[NUM_CLIENTS];
  sem_t* done;
} ClientsState;

ClientsState clients;

typedef struct {
  int line_number;
  sem_t* lock;
  sem_t* client_wants_to_pay;
  sem_t* ready_for_client[NUM_CLIENTS];
} CashierLineState;

CashierLineState cashier_line;

void InitSemaphores() {
  manager.new_waffle_ready = malloc(sizeof(sem_t));
  manager.waffle_checked = malloc(sizeof(sem_t));
  manager.lock = malloc(sizeof(sem_t));
  clients.done = malloc(sizeof(sem_t));
  cashier_line.lock = malloc(sizeof(sem_t));
  cashier_line.client_wants_to_pay = malloc(sizeof(sem_t));
  assert(manager.new_waffle_ready != NULL);
  assert(manager.waffle_checked != NULL);
  assert(manager.lock != NULL);
  assert(clients.done != NULL);
  assert(cashier_line.lock != NULL);
  assert(cashier_line.client_wants_to_pay != NULL);  
  sem_init(manager.new_waffle_ready, /*pshared=*/0, /*value=*/0);
  sem_init(manager.waffle_checked, /*pshared=*/0, /*value=*/0);
  sem_init(manager.lock, /*pshared=*/0, /*value=*/1);
  sem_init(clients.done, /*pshared=*/0, /*value=*/0);
  sem_init(cashier_line.lock, /*pshared=*/0, /*value=*/1);
  sem_init(cashier_line.client_wants_to_pay, /*pshared=*/0, /*value=*/0);
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    cashier_line.ready_for_client[i] = malloc(sizeof(sem_t*));
    assert(cashier_line.ready_for_client[i] != NULL);
    sem_init(cashier_line.ready_for_client[i], /*pshared=*/0, /*value=*/0);
  }
  printf("Semaphores initialized.\n");
}

void DestroySemaphores() {

  sem_destroy(manager.new_waffle_ready);
  sem_destroy(manager.waffle_checked);
  sem_destroy(manager.lock);
  sem_destroy(clients.done);
  sem_destroy(cashier_line.lock);
  free(manager.new_waffle_ready);
  free(manager.waffle_checked);
  free(manager.lock);
  free(clients.done);
  free(cashier_line.lock);
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    sem_destroy(cashier_line.ready_for_client[i]);
    free(cashier_line.ready_for_client[i]);
  }

  printf("Semaphores destroyed.\n");
}

void* Manager(void* arg) {
  printf("MANAGER: running.\n");
  int num_waffles = *(int*)arg;
  int num_approved = 0;
  int num_checked = 0;
  while (num_approved < num_waffles) {
    if (sem_wait(manager.new_waffle_ready) == -1) {
      // Thread might wake not only when semaphore unlocks but when thread
      // gets interruped by a signal handler. See sem_wait documentation for
      // more details.
      assert(errno == EINTR);
      continue; // Try again.
    }
    manager.approved = rand() % 2;
    sem_post(manager.waffle_checked);
    ++num_checked;
    num_approved += manager.approved;
    printf("MANAGER: checked new waffle, status %d.\n", manager.approved);
  }
  printf("MANAGER: done.\n");
  return NULL;
}

void MakeNewWaffle() {
  sleep(rand() % 5);
  printf("WAFFLE_MAKER: made new waffle (we do not know for "
   "which client though, could be passed as an arg, feel "
   "free to send me a pull request  :).\n");
}

void* WaffleMaker(void* arg) {
  sem_t* signal_client = arg;
  bool good = false;
  while (!good) {
    MakeNewWaffle();
    while (sem_wait(manager.lock) == -1) {
      assert(errno == EINTR);
    }
    sem_post(manager.new_waffle_ready);
    while (sem_wait(manager.waffle_checked) == -1) {
      assert(errno == EINTR);
    }
    good = manager.approved;
    sem_post(manager.lock);
  }
  sem_post(signal_client);
  return NULL;
}

void* Client(void* arg) {
  const int id = *(int*)arg;
  printf("CLIENT %d: running.\n", id);
  const int num_waffles = clients.per_client_waffles[id];
  printf("CLIENT %d: needs %d waffles to become unicorn.\n", id, num_waffles);
  sem_t* waffle_approved = NULL;

  waffle_approved = malloc(sizeof(sem_t));
  assert(waffle_approved != NULL);
  sem_init(waffle_approved, /*pshared=*/0, /*value=*/0);
  for (int i = 0; i < num_waffles; ++i) {
    pthread_t tid;
    pthread_create(&tid, /*attr=*/NULL, WaffleMaker, waffle_approved);
    pthread_detach(tid);
    printf("CLIENT %d: scheduled new WaffleMaker.\n", id);
  }
  for (int i = 0; i < num_waffles; ++i) {
    if (sem_wait(waffle_approved) == -1) {
      assert(errno == EINTR);
      --i;
      continue;
    }
    printf("CLIENT %d: received new waffle.\n", id);
  }
  printf("CLIENT %d: all waffles received, ready to pay.\n", id);
  while (sem_wait(cashier_line.lock) == -1) {
    assert(errno == EINTR);
    continue;
  }
  int line_number = cashier_line.line_number;
  ++cashier_line.line_number;
  sem_post(cashier_line.lock);
  printf("CLIENT %d: got cashier line number %d.\n", id, line_number);
  sem_post(cashier_line.client_wants_to_pay);
  printf("CLIENT %d: notified cashier.\n", id);
  while (sem_wait(cashier_line.ready_for_client[line_number]) == -1) {
    assert(errno == EINTR);
    continue;
  }
  printf("CLIENT %d: paying for waffles.\n", id);
  printf("CLIENT %d: going home well fed.\n", id);

  sem_destroy(waffle_approved);
  free(waffle_approved);
  sem_post(clients.done);
  return NULL;
}

void TakeMoney() {
  sleep(rand() % 5);
}

void* Cashier(void* arg) {
  printf("CASHIER: running.\n");
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    while (sem_wait(cashier_line.client_wants_to_pay) == -1) {
      assert(errno == EINTR);
      continue;
    }
    printf("CASHIER: ready for client %d.\n", i);
    sem_post(cashier_line.ready_for_client[i]);
    TakeMoney();
    printf("CASHIER: took money from client %d.\n", i);
  }
  printf("CASHIER: done.\n");
  return NULL;
}

int main(int argc, char* argv[]) {
  InitSemaphores();
  int total_num_waffles = 0;
  pthread_t tid;
  int client_ids[NUM_CLIENTS];
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    client_ids[i] = i;
    clients.per_client_waffles[i] = rand() % MAX_WAFFLES_PER_CLIENT + 1;
    total_num_waffles += clients.per_client_waffles[i];
    pthread_create(&tid, /*attr=*/NULL, Client, &client_ids[i]);
    // We don't care joining on this thread in this simulation. Instead we'll
    // receive signal on clients.done semaphore.
    pthread_detach(tid);
    printf("MAIN: scheduled new client %d.\n", i);
  }
  pthread_create(&tid, /*attr=*/NULL, Manager, &total_num_waffles);
  pthread_detach(tid);
  pthread_create(&tid, /*attr=*/NULL, Cashier, /*arg=*/NULL);
  for (int i = 0; i < NUM_CLIENTS; ++i) {
    while (sem_wait(clients.done) == -1) {
      assert(errno == EINTR);
      continue;
    }
  }
  printf("MISSION ACCOMPLISHED: all clients have been turned into fabulous unicorns ...\n");
  DestroySemaphores();
  return 0;
}