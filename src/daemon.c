#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>

void signal_handler(sig)
	int sig;
{
	switch(sig) {
		case SIGHUP:
			// log_message(LOG_FILE,"hangup signal catched");
			syslog(LOG_INFO, "caught hangup ... ");
			break;
		case SIGTERM:
			syslog(LOG_INFO, "caught terminate ... stopping gracefully");
			exit(0);
			break;
	}
}


void daemonize(char *rundir, char *pidfile)
{
	FILE *fp= NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	process_id = fork();

	if (process_id < 0) {
		syslog(LOG_ERROR, "unable to fork");
		exit(1);
	}

	if (process_id > 0) {
		fp = fopen (pidfile, "w+");
		fprintf(fp, "%d", process_id);
		fflush(fp);
		fclose(fp);
		exit(0);
	}

	umask(022);

	if( (sid = setsid()) < 0) {
		syslog(LOG_ERROR, "unable to obtain new session");
		exit(1);
	}

	if ( chdir(rundir) != 0 ) {
		syslog(LOG_ERROR, "Unable to chdir");
		exit(1);
	}

	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	signal(SIGCHLD, SIG_IGN); /* ignore child */
	signal(SIGTSTP, SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGHUP,  signal_handler); /* catch hangup signal */
	signal(SIGTERM, signal_handler); /* catch kill signal */
	return (0);
}


int main(int argc, char* argv[]) {
	daemonize("/var/run/daemon", "/var/run/daemon/daemon.pid");
	while(1) {
		// do stuff
	}
}
