#ifndef _TIMER_LIST_H_
#define _TIMER_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#define MAX_TIMER_NUM		1000	/**< max timer number	*/
#define TIMER_START 		1	/**< timer start(second)*/
#define TIMER_TICK 		1	/**< timer tick(second)	*/
#define INVALID_TIMER_ID 	(-1)	/**< invalid timer ID	*/

#ifndef TRUE
#define TRUE 	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

typedef int timer_id;

/**
 * The type of callback function to be called by timer scheduler when a timer
 * has expired.
 *
 * @param id		The timer id.
 * @param user_data     The user data.
 * $param len		The length of user data.
 */
typedef int timer_expiry(timer_id id, void *user_data, int len);

/**
 * The type of the timer
 */
struct timer {
	LIST_ENTRY(timer) entries;	/**< list entry		*/	
	
	timer_id id;			/**< timer id		*/

	int interval;			/**< timer interval(second)*/
	int elapse; 			/**< 0 -> interval 	*/

	timer_expiry *cb;		/**< call if expiry 	*/
	void *user_data;		/**< callback arg	*/
	int len;			/**< user_data length	*/
};

/**
 * The timer list
 */
struct timer_list {
	LIST_HEAD(listheader, timer) header;	/**< list header 	*/
	int num;								/**< timer entry number */
	int max_num;							/**< max entry number	*/

	void (*old_sigfunc)(int);				/**< save previous signal handler */
	void (*new_sigfunc)(int);				/**< our signal handler	*/

	struct itimerval ovalue;				/**< old timer value */
	struct itimerval value;					/**< our internal timer value */
};

static struct timer_list timer_list;

extern int init_timer(int count);

extern int destroy_timer(void);

extern timer_id 
add_timer(int interval, timer_expiry *cb, void *user_data, int len);

extern int del_timer(timer_id id);

static void sig_func(int signo);

extern  char *fmt_time(char *tstr);


#ifdef __cplusplus
	}
#endif

#endif