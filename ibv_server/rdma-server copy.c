// NOTE: This is a modified version of rdma-server.c for a single direction communication.
// NOTENOTE: This version is then updated for the small MR reuse / flow control model
#include "rdma-common.h"

static int on_connect_request(struct rdma_cm_id *id);
static int on_connection(struct rdma_cm_id *id);
static int on_disconnect(struct rdma_cm_id *id);
static int on_event(struct rdma_cm_event *event);
//static void usage(const char *argv0);
//long RDMA_BUFFER_SIZE;

int main(int argc, char **argv)
{
  struct sockaddr_in6 addr;
  struct rdma_cm_event *event = NULL;
  struct rdma_cm_id *listener = NULL;
  struct rdma_event_channel *ec = NULL;
  uint16_t port = 0;

  // Now we don't need any command line args for the server.
  /*
  if (argc != 3)
    usage(argv[0]);

  RDMA_BUFFER_SIZE = atol(argv[2]);

  if (strcmp(argv[1], "write") == 0)
    set_mode(M_WRITE);
  else if (strcmp(argv[1], "read") == 0)
    set_mode(M_READ);
  else
    usage(argv[0]);
  */

  memset(&addr, 0, sizeof(addr));
  addr.sin6_family = AF_INET6;

  TEST_Z(ec = rdma_create_event_channel());
  TEST_NZ(rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP));
  TEST_NZ(rdma_bind_addr(listener, (struct sockaddr *)&addr));
  TEST_NZ(rdma_listen(listener, 10)); /* backlog=10 is arbitrary */

  port = ntohs(rdma_get_src_port(listener));

  printf("listening on port %d.\n", port);

  while (rdma_get_cm_event(ec, &event) == 0) {
    struct rdma_cm_event event_copy;

    memcpy(&event_copy, event, sizeof(*event));
    rdma_ack_cm_event(event);

    if (on_event(&event_copy))
      break;
  }

  rdma_destroy_id(listener);
  rdma_destroy_event_channel(ec);

  return 0;
}

int on_connect_request(struct rdma_cm_id *id)
{
  struct rdma_conn_param cm_params;

  printf("received connection request.\n");
  build_connection(id);
  build_params(&cm_params);
//  sprintf(get_local_message_region(id->context), "message from passive/server side with pid %d", getpid());
  TEST_NZ(rdma_accept(id, &cm_params));

  return 0;
}

int on_connection(struct rdma_cm_id *id)
{
  on_connect(id->context);
  //send_mr(id->context);  // send MR_key to the client ////NOTE
  return 0;
}

int on_disconnect(struct rdma_cm_id *id)
{
  printf("peer disconnected.\n");

  destroy_connection(id->context);
  return 0;
}

int on_event(struct rdma_cm_event *event)
{
  int r = 0;

  if (event->event == RDMA_CM_EVENT_CONNECT_REQUEST)
    r = on_connect_request(event->id);
  else if (event->event == RDMA_CM_EVENT_ESTABLISHED)
    r = on_connection(event->id);
  else if (event->event == RDMA_CM_EVENT_DISCONNECTED)
    r = on_disconnect(event->id);
  else
    die("on_event: unknown event.");

  return r;
}

//void usage(const char *argv0)
//{
//  fprintf(stderr, "usage: %s <mode> size\n  mode = \"read\", \"write\"\n", argv0);
//  exit(1);
//}
