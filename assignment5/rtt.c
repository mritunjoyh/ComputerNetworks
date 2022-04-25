#include <curl/curl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  CURLcode ret;
  CURL *hnd;

  hnd = curl_easy_init();
  curl_easy_setopt(hnd, CURLOPT_BUFFERSIZE, 102400L);
  curl_easy_setopt(hnd, CURLOPT_URL, argv[1]);
  curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.58.0");
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);

  FILE *f;
  f = fopen("rtt.txt", "w");
  for(int i = 0; i < atoi(argv[2]); ++i) {
	  struct timespec start, end;
	  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
	  ret = curl_easy_perform(hnd);
	  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
	  double delta = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%lf\n",delta);
	  fprintf(f, "%lf\n", delta);
  }
  fclose(f);
  curl_easy_cleanup(hnd);
  hnd = NULL;
  int status;
  char *args[2];
  args[1] = "python plot.py";
  args[2] = NULL;
  if(fork()==0)
  {

    char *pythonIntrepreter="python";
    char *calledPython="./plot.py";
    char *pythonArgs[]={pythonIntrepreter,calledPython,"a","b","c",NULL};
    execvp(pythonIntrepreter,pythonArgs);

  }
  wait(&status);
  return (int)ret;
}