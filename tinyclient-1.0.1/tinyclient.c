/*

 tinyclient - an example MUD client

 Author: 	Nick Gammon <nick@gammon.com.au>
          http://www.gammon.com.au/

 Date:		8th January 2003

 This program is placed in the public domain.

 You may use it as a starting point for writing your own MUD client.

 I would appreciated being credited in the event that you use this code.

 Please do not email questions directly to me, send questions/comments to my
 forum at:

 http://www.gammon.com.au/forum/
 
 Because the program is in the public domain, there is no warranty for the program,
 to the extent permitted by applicable law.

 The author provides the program "as is" without warranty of any kind, either
 expressed or implied, including, but not limited to, the implied warranties of
 merchantability and fitness for a particular purpose.

 The entire risk as to the quality and performance of the program is with you.

 Should the program prove defective, you assume the cost of all necessary servicing,
 repair or correction.

 To compile without using the makefile:

   gcc tinyclient.c -o tinyclient -g -Wall

 Version History
 ---------------

 8 Jan 2003 - Initial release (1.0.0)
 8 Jan 2003 - Removed some commented-out code (1.0.1)
 
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

#define UMIN(a, b)              ((a) < (b) ? (a) : (b))
#define UMAX(a, b)              ((a) > (b) ? (a) : (b))
#define VERSION "1.0.1"						// client version

// change this stuff to customise behaviour (eg. where to connect, what port)

#define HOSTNAME 					"localhost"					// where to connect to (name or IP)
#define PORT 							4000								// port to connect to
#define INITIAL_STRING 		"lordrom\nlordrom\n"  // initial connect string
#define FINAL_STRING 			"quit\n"  				  // wrap-up string

#define COMMAND 					"sigh\nmoan\n"  		// idle command
#define COMMAND_INTERVAL 	10     							// seconds idle before sending command

#define COMMS_WAIT_SEC  	0    								/* time to wait in seconds */
#define COMMS_WAIT_USEC 	500000    					/* time to wait in microseconds */

static int bStopNow = 0;			// set by signal handler

time_t tLastSend;							// time we last sent something to the MUD

//  sends a message

void sendit (int s, const char * msg)
{
  if ( send (s, msg, strlen (msg), 0) == -1)
    perror ("send");
  else
    tLastSend = time (NULL);

}

// receives asynchronous output, waits COMMS_WAIT_SEC/COMMS_WAIT_USEC and then returns

// returns: -1 = error
//           0 = timeout
//           1 = connection closed

int processoutput (int s)
{

fd_set in_set;
fd_set exc_set;
struct timeval timeout;
static char buf [1000];
  
  // loop processing input

for ( ; ; )
    {

    FD_ZERO( &in_set  );
    FD_ZERO( &exc_set );

    // add our socket
    FD_SET( s, &in_set  );
    FD_SET( s, &exc_set );

    // and stdin
    FD_SET( STDIN_FILENO, &in_set  );
    FD_SET( STDIN_FILENO, &exc_set );

    timeout.tv_sec = COMMS_WAIT_SEC;
    timeout.tv_usec = COMMS_WAIT_USEC;  /* wait half a second */

    if (select (UMAX (s, STDIN_FILENO) + 1, &in_set, NULL, &exc_set, &timeout) == 0)
      return 0;		// time  limit expired? return

    // check for exception on our socket
    
    if (FD_ISSET (s, &exc_set))
      {
      printf ("Exception occurred on socket\n");
      return -1;	// out of receive loop
      }

    // check for input on our socket
    
    if (FD_ISSET (s, &in_set))
      {
      int nRead;

      nRead = read (s, buf, sizeof(buf) - 1);

      // input but zero length? must have closed the connection
      
      if (nRead == 0)
        {
        printf ("Connection closed\n");
        return 1;
        }

      // less than zero bytes? error on connection
      
      if (nRead < 0)
        {
        perror ("read from MUD");
        return -1;
        }

      // terminate buffer, display results
      
      buf [nRead] = 0;  // terminating null
      printf ("%s", buf);
      fflush (stdout);

      }  // end of having some input for this socket

    // check for input on the keyboard

    if (FD_ISSET (STDIN_FILENO, &in_set))
      {
      int nRead;

      nRead = read (STDIN_FILENO, buf, sizeof(buf) - 1);
      
      // less than zero bytes? error on stdin

      if (nRead < 0)
        {
        perror ("input from player");
        return -1;
        }

      // terminate buffer, send results

      buf [nRead] = 0;  // terminating null
      sendit (s, buf);
        
      }  // end of having some input from the keyboard
    
    } // end of looping processing input

}

void bailout (int sig)
{
  printf ("**** Terminated by player on signal %i ****\n\n", sig);
  bStopNow = 1;
}

int main(int argc, char* argv[])
{

int s;  // socket
struct sockaddr_in sa; 	

	printf ("Tinyclient version %s\n", VERSION);
  printf ("Connecting to %s, port %i\n", HOSTNAME, PORT);

  /* standard termination signals */
  signal(SIGINT, bailout);
  signal(SIGTERM, bailout);
  signal(SIGHUP, bailout);

  //
  // Create a TCP/IP stream socket
  //
  if ( (s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
      {
      perror ("creating socket");
      return 1;
      }

  //
  // Fill in the the address structure
  //
  memset(&sa, '\0', sizeof(sa));
  sa.sin_port = htons (PORT);
  sa.sin_family = AF_INET;

  // look up IP address, convert into number
  
  if ((sa.sin_addr.s_addr=inet_addr (HOSTNAME)) == INADDR_NONE)
    {
    struct hostent *hostinfo;

    if (NULL == (hostinfo = gethostbyname (HOSTNAME)))
      {
      herror ("finding hostname: " HOSTNAME);
      return 1;
      }

    sa.sin_addr.s_addr = *(unsigned long *) hostinfo->h_addr;
    }

  //
  // connect to the socket
  //
  if ( connect (s, (struct sockaddr *) &sa, sizeof sa) == -1)
    {
    perror ("connecting to MUD");
    return 1;
    }

  printf ("Connected using socket %i.\n", s);

  sendit (s, INITIAL_STRING);

  // get response to initial string
  processoutput (s);

// loop processing MUD input, and our keystrokes
  
  do
    {

    // process incoming comms, and commands from player
    if (processoutput (s) != 0)
      break;
      
    // send new command if it is time
    if (time (NULL) > (tLastSend + COMMAND_INTERVAL))
      sendit (s, COMMAND);

    } while (!bStopNow);

  // wrap up (quit)
  sendit (s, FINAL_STRING);

  // get response to quit
  processoutput (s);

  //
  // Close the socket
  //
  close (s);

  // wrap up
	return 0;
}
