/*
 * NAME
 *   gpio_input.c (V2)
 *
 * DESCRIPTION
 *
 * AUTHOR
 *   DLS, 06/24
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#ifdef __linux__
#include <stdbool.h>
#include <linux/types.h>
#include <linux/gpio.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#endif

#include <tcl.h>

#include "Datapoint.h"
#include "tclserver_api.h"

#ifdef __linux__
/* helper functions for gpio_v2_line_values bits */
static inline void gpiotools_set_bit(__u64 *b, int n)
{
  *b |= _BITULL(n);
}

static inline void gpiotools_change_bit(__u64 *b, int n)
{
  *b ^= _BITULL(n);
}

static inline void gpiotools_clear_bit(__u64 *b, int n)
{
  *b &= ~_BITULL(n);
}

static inline int gpiotools_test_bit(__u64 b, int n)
{
  return !!(b & _BITULL(n));
}

static inline void gpiotools_assign_bit(__u64 *b, int n, bool value)
{
  if (value)
    gpiotools_set_bit(b, n);
  else
    gpiotools_clear_bit(b, n);
}
#endif

typedef struct gpio_input_s
{
  int line;
#ifdef __linux__  
  //  struct gpioevent_request req;
  struct gpio_v2_line_request req;
  struct epoll_event ev;
  pthread_t input_thread_id;
  int epfd;			/* epoll fd */
#endif
  tclserver_t *tclserver;
  char *dpoint_prefix;
  int debounce_period_us;
  int val;
} gpio_input_t;
  
typedef struct gpio_info_s
{
  int fd;			/* chip fd */
  int nlines;
  tclserver_t *tclserver;
  char *dpoint_prefix;
#ifdef __linux__
  gpio_input_t **input_requests;
#endif
} gpio_info_t;

/* global to this module */
static gpio_info_t g_gpioInfo;


#ifdef __linux__

void *input_thread(void *arg)
{
  gpio_input_t *info = (gpio_input_t *) arg;
  struct gpio_v2_line_values lv;
  struct epoll_event ev;
  
  ev.events = EPOLLIN;
  ev.data.fd = info->req.fd;
  info->epfd = epoll_create(1);
  int res = epoll_ctl(info->epfd, EPOLL_CTL_ADD, info->req.fd, &ev);
  int nfds, nread;

  char point_name[64];
  sprintf(point_name, "%s/%d", info->dpoint_prefix, info->line);
  int status, ret, val;
  ds_datapoint_t *dp;

  /* get initial value and set in dataserver */
  gpiotools_set_bit(&lv.mask, 0); /* 0 -> only one line */
  ret = ioctl(info->req.fd, GPIO_V2_LINE_GET_VALUES_IOCTL, &lv);
  if (ret != -1) {
    val = gpiotools_test_bit(lv.bits, 0);

    /* create a new datapoint, memory managed by Dataserver */
    dp = dpoint_new(point_name, tclserver_now(info->tclserver),
		    DSERV_INT, sizeof(int), (unsigned char *) &val);
    tclserver_set_point(info->tclserver, dp);
  }
  
  while (1) {
    nfds = epoll_wait(info->epfd, &ev, 1, 20000);
    if (nfds != 0) {
      struct gpio_v2_line_event event;
      nread = read(info->req.fd, &event, sizeof(event));

      if (nread == -1) {
	if (errno == -EAGAIN) {
	  //	  fprintf(stderr, "nothing available\n");
	  continue;
	} else {
	  //	  ret = -errno;
	  //	  fprintf(stderr, "Failed to read event (%d)\n", ret);
	  break;
	}
      }
      
      if (nread != sizeof(event)) {
	// fprintf(stderr, "Reading event failed\n");
	//ret = -EIO;
	break;
      }
      
      status = (event.id == GPIO_V2_LINE_EVENT_RISING_EDGE) ? 1 : 0;
      
      dp = dpoint_new(point_name,
		      tclserver_now(info->tclserver),
		      DSERV_INT, sizeof(int),
		      (unsigned char *) &status);
      tclserver_set_point(info->tclserver, dp);

      // fprintf(stdout, "GPIO EVENT at %" PRIu64 " on line %d (%d|%d) ",
      //        (uint64_t)event.timestamp_ns, event.offset, event.line_seqno,
      //		event.seqno);
    }
  }
}

static int shutdown_input_thread(gpio_input_t *ireq)
{
  pthread_cancel(ireq->input_thread_id);
  close(ireq->epfd);
  close(ireq->req.fd);
  return 0;
}


static int gpio_input_init_command(ClientData data,
				   Tcl_Interp *interp,
				   int objc, Tcl_Obj *objv[])
{
  int chipnum;
  char *chipstr;
  char chipstr_buf[128];
  
  gpio_info_t *info = (gpio_info_t *) data;
  int ret;

  if (objc < 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "chipnum|chipname");
    return TCL_ERROR;
  }

  if (Tcl_GetIntFromObj(interp, objv[1], &chipnum) == TCL_OK) {
    snprintf(chipstr_buf, sizeof(chipstr_buf), "/dev/gpiochip%d", chipnum);
    chipstr = (char *) &chipstr_buf;
  }
  else {
    Tcl_ResetResult(interp);
    chipstr = Tcl_GetString(objv[1]);
  }

  if (info->fd >= 0) return TCL_OK; /* should clean up and allow to open */
  
  info->fd = open(chipstr, O_RDONLY);
  if (info->fd < 0) {
    Tcl_AppendResult(interp, "error opening gpio chip", chipstr, NULL);
    return TCL_ERROR;
  }

  struct gpiochip_info gpioinfo;  
  ret = ioctl(info->fd, GPIO_GET_CHIPINFO_IOCTL, &gpioinfo);
    
  if (ret >= 0) {
    info->nlines = gpioinfo.lines;
    info->input_requests =
      (gpio_input_t **) calloc(gpioinfo.lines, sizeof(struct gpio_input_t *));
  }
  else {
    info->nlines = 0;
    info->input_requests = NULL;
  }

  Tcl_SetObjResult(interp, Tcl_NewIntObj(ret));
  return TCL_OK; 
}

static int gpio_line_request_input_command(ClientData data,
				    Tcl_Interp *interp,
				    int objc, Tcl_Obj *objv[])
{
  gpio_info_t *info = (gpio_info_t *) data;
  int offset = 0;
  int debounce_period_us = 0;
  uint32_t bias_flag   = GPIO_V2_LINE_FLAG_BIAS_PULL_UP;
  uint32_t active_flag = GPIO_V2_LINE_FLAG_ACTIVE_LOW;
  uint32_t edge_flag = GPIO_V2_LINE_FLAG_EDGE_RISING | GPIO_V2_LINE_FLAG_EDGE_FALLING;
  int attr;
  int ret, val;
    
  if (info->fd < 0) {
    return TCL_OK;
  }
  
  if (objc < 2) {
    Tcl_WrongNumArgs(interp, 1, objv,
		     "offset [RISING|FALLING|BOTH] [debounce_us] [PULL_UP|PULL_DOWN] [ACTIVE_LOW|ACTIVE_HIGH]");
    return TCL_ERROR;
  }

  /* check all args first */
  if (Tcl_GetIntFromObj(interp, objv[1], &offset) != TCL_OK) {
    return TCL_ERROR;
  }
  if (offset >= info->nlines) {
    Tcl_AppendResult(interp, "invalid line specified for input (",
		     Tcl_GetString(objv[1]), ")",
		     NULL);
    return TCL_ERROR;
  }

  if (objc > 2) {
    if (!strcmp(Tcl_GetString(objv[2]), "BOTH"))
      edge_flag = GPIO_V2_LINE_FLAG_EDGE_RISING |
	GPIO_V2_LINE_FLAG_EDGE_FALLING;
    else if (!strcmp(Tcl_GetString(objv[2]), "FALLING"))
      edge_flag = GPIO_V2_LINE_FLAG_EDGE_FALLING;
    else if (!strcmp(Tcl_GetString(objv[2]), "RISING"))
      edge_flag = GPIO_V2_LINE_FLAG_EDGE_RISING;
  }

  if (objc > 3) {
    if (Tcl_GetIntFromObj(interp, objv[3], &debounce_period_us) != TCL_OK) {
      return TCL_ERROR;
    }
  }

  if (objc > 4) {
    if (!strcmp(Tcl_GetString(objv[4]), "PULL_UP") ||
	!strcmp(Tcl_GetString(objv[4]), "pull_up")) {
      bias_flag = GPIO_V2_LINE_FLAG_BIAS_PULL_UP;
    }
    else if (!strcmp(Tcl_GetString(objv[4]), "PULL_DOWN") ||
	!strcmp(Tcl_GetString(objv[4]), "pull_down")) {
      bias_flag = GPIO_V2_LINE_FLAG_BIAS_PULL_DOWN;
    }
    else if (!strcmp(Tcl_GetString(objv[4]), "PULL_NONE") ||
	!strcmp(Tcl_GetString(objv[4]), "pull_none")) {
      bias_flag = GPIO_V2_LINE_FLAG_BIAS_DISABLED;
    }
    else {
      Tcl_AppendResult(interp, Tcl_GetString(objv[0]),
		       ": bias must be PULL_UP|PULL_DOWN|PULL_NONE", NULL);
      return TCL_ERROR;
    }
  }

  if (objc > 5) {
    if (!strcmp(Tcl_GetString(objv[5]), "ACTIVE_LOW") ||
	!strcmp(Tcl_GetString(objv[5]), "active_low")) {
      active_flag = GPIO_V2_LINE_FLAG_ACTIVE_LOW;
    }
    else if (!strcmp(Tcl_GetString(objv[5]), "ACTIVE_HIGH") ||
	!strcmp(Tcl_GetString(objv[5]), "ACTIVE_HIGH")) {
      active_flag = 0;
    }
    else {
      Tcl_AppendResult(interp, Tcl_GetString(objv[0]),
		       ": active_flag be ACTIVE_HIGH|ACTIVE_LOW", NULL);
      return TCL_ERROR;
    }
  }



  gpio_input_t *ireq = info->input_requests[offset];
  if (ireq) {		/* already opened, so close */
    shutdown_input_thread(ireq);
    /* wait for thread to shutdown */
    pthread_join(ireq->input_thread_id, NULL);
  }
  else {
    ireq = info->input_requests[offset] = (gpio_input_t *)
      calloc(1, sizeof(gpio_input_t));
  }

  /* clear request and configure for desired line */
  memset(&ireq->req, 0, sizeof(ireq->req));
  ireq->req.offsets[0] = offset;
  ireq->req.num_lines = 1;
  
  /* setup config for this request (overridden by LINE_ATTR_ID_FLAGS) */
  memset(&ireq->req.config, 0, sizeof(ireq->req.config));
  ireq->req.config.flags = GPIO_V2_LINE_FLAG_INPUT;
  ireq->req.config.flags |= edge_flag;

  /* add attributes for BIAS (pull up / pull down ) and ACTIVE state */
  attr = ireq->req.config.num_attrs;
  ireq->req.config.num_attrs++;
  gpiotools_set_bit(&ireq->req.config.attrs[attr].mask, 0);
  ireq->req.config.attrs[attr].attr.id = GPIO_V2_LINE_ATTR_ID_FLAGS;
  ireq->req.config.attrs[attr].attr.flags =
    bias_flag | active_flag | edge_flag | GPIO_V2_LINE_FLAG_INPUT;
  
  /* add an attribute for debounce */
  if (debounce_period_us) {
    attr = ireq->req.config.num_attrs;
    ireq->req.config.num_attrs++;
    gpiotools_set_bit(&ireq->req.config.attrs[attr].mask, 0);
    ireq->req.config.attrs[attr].attr.id = GPIO_V2_LINE_ATTR_ID_DEBOUNCE;
    ireq->req.config.attrs[attr].attr.debounce_period_us = debounce_period_us;
  }

  /* set consumer name */
  strncpy(ireq->req.consumer, "dserv input",
	  sizeof(ireq->req.consumer));
  
  ireq->tclserver = info->tclserver;
  ireq->dpoint_prefix = info->dpoint_prefix; /* belongs to global */
  ireq->line = offset;
  if (debounce_period_us >= 0) {
    ireq->debounce_period_us = debounce_period_us;
  }
  
  ret = ioctl(info->fd, GPIO_V2_GET_LINE_IOCTL, &ireq->req);
  if (ret == -1) {
    printf("ioctl GPIO_V2_GET_LINE_IOCTL error: %d\n", errno);
  }
  if (ret != -1) {
    if (pthread_create(&ireq->input_thread_id, NULL, input_thread,
		       (void *) ireq)) {
      return TCL_ERROR;
    }
  }
  else {			/* failed to get line */
    free(ireq);
    info->input_requests[offset] = NULL;
  }

  Tcl_SetObjResult(interp, Tcl_NewIntObj(ret));
  return TCL_OK;
}


static int gpio_line_release_input_command(ClientData data,
				    Tcl_Interp *interp,
				    int objc, Tcl_Obj *objv[])
{
  gpio_info_t *info = (gpio_info_t *) data;
  int offset = 0;
  
  if (info->fd < 0) {
    return TCL_OK;
  }
  
  if (objc < 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "offset");
    return TCL_ERROR;
  }

  /* check all args first */
  if (Tcl_GetIntFromObj(interp, objv[1], &offset) != TCL_OK) {
    return TCL_ERROR;
  }
  if (offset >= info->nlines) {
    Tcl_AppendResult(interp, "invalid line specified (",
		     Tcl_GetString(objv[1]), ")",
		     NULL);
    return TCL_ERROR;
  }

  gpio_input_t *ireq = info->input_requests[offset];
  if (ireq) {		/* already opened, so close */
    shutdown_input_thread(ireq);
    /* wait for thread to shutdown */
    pthread_join(ireq->input_thread_id, NULL);
    free(ireq);
    info->input_requests[offset] = NULL;
    Tcl_SetObjResult(interp, Tcl_NewIntObj(offset));
  }
  else {
    Tcl_SetObjResult(interp, Tcl_NewIntObj(-1));
  }
  return TCL_OK;
}

static int gpio_line_release_all_inputs_command(ClientData data,
						Tcl_Interp *interp,
						int objc, Tcl_Obj *objv[])
{
  gpio_info_t *info = (gpio_info_t *) data;
  int offset;
  gpio_input_t *ireq;
  int nreleased = 0;
  
  if (info->fd < 0) {
    return TCL_OK;
  }

  /* loop through all lines and release if had been requested */
  for (offset = 0; offset < info->nlines; offset++) {
    ireq = info->input_requests[offset];
    if (ireq) {		/* already opened, so close */
      shutdown_input_thread(ireq);
      /* wait for thread to shutdown */
      pthread_join(ireq->input_thread_id, NULL);
      free(ireq);
      info->input_requests[offset] = NULL;
      nreleased++;
    }
  }
  Tcl_SetObjResult(interp, Tcl_NewIntObj(nreleased));
  return TCL_OK;
}

#else
static int gpio_input_init_command(ClientData data,
				    Tcl_Interp *interp,
				    int objc, Tcl_Obj *objv[])
{
  return TCL_OK;
}
 
 static int gpio_line_request_input_command(ClientData data,
					    Tcl_Interp *interp,
				    int objc, Tcl_Obj *objv[])
{
  int offset = 0;
  int debounce_period_us = 0;
  uint32_t bias_flag;
  uint32_t active_flag;
  uint32_t edge_flag;

  if (objc < 2) {
    Tcl_WrongNumArgs(interp, 1, objv,
		     "offset [RISING|FALLING|BOTH] [debounce_us] [PULL_UP|PULL_DOWN] [ACTIVE_LOW|ACTIVE_HIGH]");
    return TCL_ERROR;
  }

  /* check all args first */
  if (Tcl_GetIntFromObj(interp, objv[1], &offset) != TCL_OK) {
    return TCL_ERROR;
  }

  if (objc > 2) {
    if (strcmp(Tcl_GetString(objv[2]), "BOTH") &&
	strcmp(Tcl_GetString(objv[2]), "FALLING") &&
	strcmp(Tcl_GetString(objv[2]), "RISING")) {
      Tcl_AppendResult(interp, "edge should be one of BOTH|FALLING|RISING", NULL);
      return TCL_ERROR;
    }
  }

  if (objc > 3) {
    if (Tcl_GetIntFromObj(interp, objv[3], &debounce_period_us) != TCL_OK) {
      return TCL_ERROR;
    }
  }

  if (objc > 4) {
    if (!strcmp(Tcl_GetString(objv[4]), "PULL_UP") ||
	!strcmp(Tcl_GetString(objv[4]), "pull_up")) {
    }
    else if (!strcmp(Tcl_GetString(objv[4]), "PULL_DOWN") ||
	!strcmp(Tcl_GetString(objv[4]), "pull_down")) {
    }
    else if (!strcmp(Tcl_GetString(objv[4]), "PULL_NONE") ||
	!strcmp(Tcl_GetString(objv[4]), "pull_none")) {
    }
    else {
      Tcl_AppendResult(interp, Tcl_GetString(objv[0]),
		       ": bias must be PULL_UP|PULL_DOWN|PULL_NONE", NULL);
      return TCL_ERROR;
    }
  }

  if (objc > 5) {
    if (!strcmp(Tcl_GetString(objv[5]), "ACTIVE_LOW") ||
	!strcmp(Tcl_GetString(objv[5]), "active_low")) {
    }
    else if (!strcmp(Tcl_GetString(objv[5]), "ACTIVE_HIGH") ||
	!strcmp(Tcl_GetString(objv[5]), "ACTIVE_HIGH")) {
    }
    else {
      Tcl_AppendResult(interp, Tcl_GetString(objv[0]),
		       ": active_flag be ACTIVE_HIGH|ACTIVE_LOW", NULL);
      return TCL_ERROR;
    }
  }

  return TCL_OK;
}

static int gpio_line_release_input_command(ClientData data,
				    Tcl_Interp *interp,
				    int objc, Tcl_Obj *objv[])
{
  int offset;
  
  if (objc < 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "offset");
    return TCL_ERROR;
  }  
  if (Tcl_GetIntFromObj(interp, objv[1], &offset) != TCL_OK) {
    return TCL_ERROR;
  }
  return TCL_OK;
}

static int gpio_line_release_all_inputs_command(ClientData data,
				    Tcl_Interp *interp,
				    int objc, Tcl_Obj *objv[])
{
  return TCL_OK;
}

#endif



/*****************************************************************************
 * EXPORT
 *****************************************************************************/

#ifdef WIN32
EXPORT(int,Dserv_gpio_input_Init) (Tcl_Interp *interp)
#else
int Dserv_gpio_input_Init(Tcl_Interp *interp)
#endif
{
  if (
#ifdef USE_TCL_STUBS
      Tcl_InitStubs(interp, "8.6-", 0)
#else
      Tcl_PkgRequire(interp, "Tcl", "8.6-", 0)
#endif
      == NULL) {
    return TCL_ERROR;
  }

  g_gpioInfo.tclserver = tclserver_get();
  g_gpioInfo.dpoint_prefix = "gpio/input";
  g_gpioInfo.fd = -1;
      
  Tcl_CreateObjCommand(interp, "gpioInputInit",
		       (Tcl_ObjCmdProc *) gpio_input_init_command,
		       &g_gpioInfo, NULL);
  Tcl_CreateObjCommand(interp, "gpioLineRequestInput",
		       (Tcl_ObjCmdProc *) gpio_line_request_input_command,
		       &g_gpioInfo, NULL);
  Tcl_CreateObjCommand(interp, "gpioLineReleaseInput",
		       (Tcl_ObjCmdProc *) gpio_line_release_input_command,
		       &g_gpioInfo, NULL);
  Tcl_CreateObjCommand(interp, "gpioLineReleaseAllInputs",
		       (Tcl_ObjCmdProc *) gpio_line_release_all_inputs_command,
		       &g_gpioInfo, NULL);
  return TCL_OK;
}
