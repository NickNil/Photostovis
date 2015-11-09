#include <stdio.h>
#include <argp.h>
#include "SyncManager.h"
#include "client.h"

const char *argp_program_version = "photostovis-0.0.1";
const char *argp_program_bug_address = "<bugss@photostovis.com>";
static char doc[] = "Photostovis - Sync your files privatly";
static char args_doc[] = "ARG1 ARG2";

static struct argp_option options[] =
{
  {"sync",     's', 0,      0,  "Synchronizes files to server in default directory" },
  {"path",     'p', 0,      0,  "Synchronizes files to server at given path" },
  {"connect",  'c', 0,      0,  "Connect to server" },
  { 0 }
};

struct arguments
{
  char *args[2];
  int silent, verbose;
  char *output_file;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key)
  {
    case 'c':
    {
      photostovis_connect_to_server("127.0.0.1", 8000);
      break;
    }
    case 's':
    {
      photostovis_sync_files_to_server();
      break;
    }
    case 'v':
    {
      arguments->verbose = 1;
      break;
    }
    case 'o':
    {
      arguments->output_file = arg;
      break;
    }
    case ARGP_KEY_ARG:
    {
      if (state->arg_num >= 2)
      {
        argp_usage (state);
        arguments->args[state->arg_num] = arg;
      }
      break;
    }
    case ARGP_KEY_END:
    {
      if (state->arg_num < 2)
      {
        argp_usage (state);
      }
      break;
    }

    default:return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv)
{
    struct arguments arguments;
    return argp_parse (&argp, argc, argv, 0, 0, &arguments);
}

