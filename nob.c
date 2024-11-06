#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"
#define  CFLAGS "-Wall", "-Wextra", "-I include/", "-L./lib"
#define LIBS "-l:libraylib.a", "-lm"

bool run_game(Nob_Cmd * cmd){
    cmd_append(cmd, "./game");
    return cmd_run_sync(*cmd);
}

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};
    cmd_append(&cmd, "cc", CFLAGS, "-o", "game", "main.c", LIBS);
    if (!cmd_run_sync(cmd))
        return 1;
    cmd.count = 0;
    if(!run_game(&cmd)) return 1;
    return 0;
}
