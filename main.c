#include <stdio.h>
#include <argp.h>
#include "SyncManager.h"
#include "client.h"
#include <stdbool.h>
#include <string.h>

const char *argp_program_version = "photostovis-0.0.1";
const char *argp_program_bug_address = "<bugss@photostovis.com>";
static char doc[] = "Photostovis - Sync your files privatly";

static char args_doc[] = "ARG1 [STRING...]";

/* Keys for options without short-options. */
#define OPT_ABORT  1            /* –abort */

/* The options we understand. */
static struct argp_option options[] = {
  {"sync",   'c', "sync",  0, "Sync files to server" },
  {"server",   's', "server",  0, "Server IP:Port, 127.0.0.1:8000" },
  {"abort",    OPT_ABORT, 0, 0, "Abort before showing any output"},

  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *arg1;                   /* arg1 */
  char **strings;               /* [string…] */
  int silent, verbose, abort;   /* ‘-s’, ‘-v’, ‘--abort’ */
  char *server;            /* file arg to ‘--output’ */
  int repeat_count;             /* count arg to ‘--repeat’ */
  char* sync;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'c' :
      arguments->sync = arg ? atoi (arg) : 0;
      break;
     case 's':
      arguments->server = arg;
      break;
    case 'r':
      arguments->repeat_count = arg ? atoi (arg) : 10;
      break;
    case OPT_ABORT:
      arguments->abort = 1;
      break;

    case ARGP_KEY_ARG:
      /* Here we know that state->arg_num == 0, since we
         force argument parsing to end before any more arguments can
         get here. */
      arguments->arg1 = arg;

      /* Now we consume all the rest of the arguments.
         state->next is the index in state->argv of the
         next argument to be parsed, which is the first string
         we’re interested in, so we can just use
         &state->argv[state->next] as the value for
         arguments->strings.

         In addition, by setting state->next to the end
         of the arguments, we can force argp to stop parsing here and
         return. */
      arguments->strings = &state->argv[state->next];
      state->next = state->argc;

      break;


    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv)
{
  struct arguments arguments;

  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.server = "-";
  arguments.repeat_count = 1;
  arguments.abort = 0;
  arguments.sync = 0;

  /* Parse our arguments; every option seen by parse_opt will be
     reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  // This means, conncet to server, should also chceck for sync argument
  if (strcmp(arguments.server,"-"))
  {
      char* server = strtok(arguments.server, ":");
      unsigned int port = atoi(strtok(NULL, ""));
      photostovis_connect_to_server(server, port);
      return 0;
  }

  if (arguments.sync)
  {
     //photostovis_get_filenames_from_client();
     printf("Syncing files to server...");
     photostovis_sync_files_to_server();
     printf("Done syncing files to server!\n");
     return 0;
  }


  return 0;
}
