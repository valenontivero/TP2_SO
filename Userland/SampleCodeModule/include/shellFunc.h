#define COMMANDS_QUANTITY 10  // ACORDARSE DE ESTO (y agregar la descripcion en commandsHelp en shellFunc.c)

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