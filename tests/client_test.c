#include<stdio.h>
#include "../client.h"


// --Ensure main() in client.c suceeds when valid arguments are given
void test_original_main(char *port)
{
    int result;
    const char *args[] = {
        "client", "--server", "localhost", "--port",
    };

    printf(" .........................................\n");
    printf(" --Testing main() with valid arguments\n");
    printf(" --\n");
    printf(" --\n");
    
    result = original_main(array_length(args), (char **) args);
    if(result==0){
        printf(" --Test Passed.\n");
    }
    else if(result==-1){
        printf(" --Port argument is not a number\n");
        printf(" --Test Failed.\n");
    }
    else{
        printf(" --Test Failed.\n");
    }
    printf(" --\n");
    printf(" --\n");
    printf(" .........................................\n");
}

// --Ensure main() in client.c fails when no arguments are given
void test_original_main_no_args()
{
    int result;
    const char *args[] = {};
    
    printf(" .........................................\n");
    printf(" --Testing main() without arguments\n");
    printf(" --\n");
    printf(" --\n");


    result = original_main(array_length(args), (char **) args);
    if(result==0){
        printf(" --Test Passed.\n");
    }
    else{
        printf(" --Test Failed.\n");
    }
    printf(" --\n");
    printf(" --\n");
    printf(" .........................................\n");
}

int main(int argc, char *argv[])
{
    //Unit Tests
    test_original_main("454545");
    test_original_main_no_args();
    return 0;
}
