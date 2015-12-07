#include <stdio.h>
#include <argp.h>
#include "SyncManager.h"
#include "FileManager.h"
#include "client.h"
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "encrypt_main.h"

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
  {"path",   'p', "path",  0, "Backup directory" },
  {"abort",    OPT_ABORT, 0, 0, "Abort before showing any output"},
  {"encrypt",   'e', "encrypt", 0, "Encrypt files before sending"},

  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *arg1;                   /* arg1 */
  char **strings;               /* [string…] */
  int silent, verbose, abort, encrypt;   /* ‘-s’, ‘-v’, ‘--abort’ , ' --encrypt'*/
  char *server;            /* file arg to ‘--output’ */
  int repeat_count;             /* count arg to ‘--repeat’ */
  char* sync;
  char* backup_file_path;
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
  case 'p' :
    arguments->backup_file_path = arg;
    break;
    case 'c' :
      arguments->sync = arg ? atoi (arg) : 0;
      break;
     case 's':
      arguments->server = arg;
      break;
    case 'r':
      arguments->repeat_count = arg ? atoi (arg) : 10;
      break;
    case 'e':
      arguments->encrypt = arg ? atoi (arg) : 1;
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

  char* server;
  unsigned int port;

  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.server = "-";
  arguments.repeat_count = 1;
  arguments.abort = 0;
  arguments.sync = 0;
  arguments.backup_file_path = "-";
  arguments.encrypt = 0;

  /* Parse our arguments; every option seen by parse_opt will be
     reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  // This means, conncet to server, should also chceck for sync argument
  int socket;
  if (strcmp(arguments.server,"-"))
  {
      server = strtok(arguments.server, ":");
      port = atoi(strtok(NULL, ""));

      socket = photostovis_connect_to_server(server, port);
      //printf("SOCKET %d", socket);
      //return 0;
  }

  if (arguments.sync)
  {
     printf("BACKUP PATH: %s", arguments.backup_file_path);
      /*
     if (strcmp(arguments.server,"-"))
     {
        printf("\nMissing backup file path! Exiting...");
        return 0;
     }
     */

     //if(arguments.encrypt)
     //{
     //  printf("\nEncrypting Files\n");
     //  photostovis_encrypt_files();
     //  printf("\nDone Encrypting files, Encrypted files saved in ./encrypted folder\n");
     //}

     char full_exe_path[1024]; // full path to executable folder
     ssize_t len = readlink("/proc/self/exe", full_exe_path, sizeof(full_exe_path)-1);
     if (len != -1) {
        full_exe_path[len-11] = '\0';
     }
     else {
       perror("ERROR: Path not found.");
     }

     photostovis_clear_backup_file(full_exe_path);
     const unsigned int pathLength = strlen(arguments.backup_file_path);
     photostovis_get_filenames_from_client(arguments.backup_file_path, pathLength, full_exe_path);
     printf("Syncing files to server...");
     if(arguments.encrypt)
     {
        printf("\nencrypt argument provided...\n");
        photostovis_sync_files_to_server(socket, server, port, arguments.backup_file_path, full_exe_path, 1);
     }
     else
     {
        photostovis_sync_files_to_server(socket, server, port, arguments.backup_file_path, full_exe_path, 0);
     }
     printf("Done syncing files to server!\n");
  }

  close(socket);

  return 0;
}
