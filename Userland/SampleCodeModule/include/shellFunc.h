#define COMMANDS_QUANTITY 23  // ACORDARSE DE ESTO (y agregar la descripcion en commandsHelp en shellFunc.c)

void help (uint8_t argc, char **argv);

void time (uint8_t argc, char **argv);

void date (uint8_t argc, char **argv);

void registers (uint8_t argc, char **argv);

void fillregs (uint8_t argc, char **argv);

void div0 (uint8_t argc, char **argv);

void invalidop (uint8_t argc, char **argv);

void pong (uint8_t argc, char **argv);

void clear (uint8_t argc, char **argv);

void hello (uint8_t argc, char **argv);

void testprint (uint8_t argc, char **argv);

void testsem (uint8_t argc, char **argv);

void testpipe (uint8_t argc, char **argv);

void testpriority (uint8_t argc, char **argv);

void mem(uint8_t argc, char **argv);

void ps(uint8_t argc, char **argv);

void loop(uint8_t argc, char **argv);

void kill(uint8_t argc, char **argv);

void nice(uint8_t argc, char **argv);

void block(uint8_t argc, char **argv);

void wc(uint8_t argc, char **argv);

void filter(uint8_t argc, char **argv);

void cat(uint8_t argc, char **argv);