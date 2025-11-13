#include <usyscalls.h>
#include <userio.h>
#include <uStrings.h>
#include <uSync.h>
#include <stdint.h>
#include <stddef.h>
#include <colors.h>
#include <processLib.h>
#include <shellFunc.h>

#define MVAR_MAX_PARTICIPANTS 10

typedef struct {
    int instanceId;
    int writerCount;
    int readerCount;
    char instanceStr[8];
    char writerCountStr[8];
    char readerCountStr[8];
    char writerIndexStr[MVAR_MAX_PARTICIPANTS][8];
    char readerIndexStr[MVAR_MAX_PARTICIPANTS][8];
    char *writerArgv[MVAR_MAX_PARTICIPANTS][5];
    char *readerArgv[MVAR_MAX_PARTICIPANTS][5];
} MVarContext;

static MVarContext currentMVar = {0};
static int mvarInstanceCounter = 0;

static const uint64_t mvarColors[] = {RED, GREEN, BLUE, YELLOW, CYAN, ORANGE, WHITE};
static const int mvarColorCount = sizeof(mvarColors) / sizeof(mvarColors[0]);

static void mvar_writer(uint8_t argc, char **argv);
static void mvar_reader(uint8_t argc, char **argv);



static void mvar_busy_wait(uint64_t loops) {
    while (loops--) {
        __asm__ volatile("nop");
    }
}
static void mvar_random_delay(int seed) {
    uint32_t ticks = 0;
    sys_get_ticks(&ticks);
    uint64_t loops = 25000000 + ((ticks + seed * 37u) % 60000);
    int prio= sys_get_prio(sys_get_pid());
    loops *= (uint64_t)(1 << (4 - prio));  
    mvar_busy_wait(loops);
}

static char* buildReaderName(int index) {
    static char name[32];
    char suffix[16] = {0};
    copyString(name, "reader_");
    intToStr(index, suffix);
    appendString(name, suffix);
    return name;
}

static char* buildWriterName(int index) {
    static char name[32];
    char suffix[16] = {0};
    copyString(name, "writer_");
    intToStr(index, suffix);
    appendString(name, suffix);
    return name;
}

static void buildMVarName(char *dest, const char *prefix, int instance) {
    char suffix[16] = {0};
    copyString(dest, prefix);
    intToStr(instance, suffix);
    appendString(dest, suffix);
}


static void buildMVarIndexedName(char *dest, const char *prefix, int instance, int index) {
    char suffix[16] = {0};
    char indexStr[8] = {0};
    copyString(dest, prefix);
    intToStr(instance, suffix);
    appendString(dest, suffix);
    appendString(dest, "_");
    intToStr(index, indexStr);
    appendString(dest, indexStr);
}

static void mvar_writer(uint8_t argc, char **argv) {
    int index = (argc > 0 && argv[1]) ? (int)atoi(argv[1]) : 0;
    int instance = (argc > 1 && argv[2]) ? (int)atoi(argv[2]) : 0;
    int writers = (argc > 2 && argv[3]) ? (int)atoi(argv[3]) : 1;

    char mutexName[32] = {0};
    char emptyName[32] = {0};
    char fullName[32] = {0};
    char pipeName[32] = {0};
    /* char turnName[32] = {0};
    char nextTurnName[32] = {0}; */

    buildMVarName(mutexName, "mvar_mutex_", instance);
    buildMVarName(emptyName, "mvar_empty_", instance);
    buildMVarName(fullName, "mvar_full_", instance);
    buildMVarName(pipeName, "mvar_pipe_", instance);
    /* buildMVarIndexedName(turnName, "mvar_turn_", instance, index);
    int nextIndex = (writers <= 0) ? index : (index + 1) % writers;
    buildMVarIndexedName(nextTurnName, "mvar_turn_", instance, nextIndex); */

    int mutexId = (int)sys_sem_open(mutexName, 0);
    int emptyId = (int)sys_sem_open(emptyName, 0);
    int fullId = (int)sys_sem_open(fullName, 0);
    int pipeId = (int)sys_pipe_open(pipeName);
    /* int turnId = (int)sys_sem_open(turnName, 0);
    int nextTurnId = (int)sys_sem_open(nextTurnName, 0);
 */
    if (mutexId < 0 || emptyId < 0 || fullId < 0 || pipeId < 0 /* || turnId < 0 || nextTurnId < 0 */) {
        printColor("mvar writer: init error\n", RED);
        return;
    }

    char letter = (char)('A' + (index % 26));

    while (1) {
        mvar_random_delay(index + 1);
        /* sys_sem_wait((uint8_t)turnId); */
        sys_sem_wait((uint8_t)emptyId);
        sys_sem_wait((uint8_t)mutexId);
        sys_pipe_write((unsigned int)pipeId, &letter, 1);
        sys_sem_post((uint8_t)mutexId);
        sys_sem_post((uint8_t)fullId);
        /* if (writers > 1) {
            sys_sem_post((uint8_t)nextTurnId);
        } else {
            sys_sem_post((uint8_t)turnId);
        } */
    }
}

static void mvar_reader(uint8_t argc, char **argv) {
    int index = (argc > 0 && argv[1]) ? (int)atoi(argv[1]) : 0;
    int instance = (argc > 1 && argv[2]) ? (int)atoi(argv[2]) : 0;
    int writers = (argc > 2 && argv[3]) ? (int)atoi(argv[3]) : 1;

    char mutexName[32] = {0};
    char emptyName[32] = {0};
    char fullName[32] = {0};
    char pipeName[32] = {0};

    buildMVarName(mutexName, "mvar_mutex_", instance);
    buildMVarName(emptyName, "mvar_empty_", instance);
    buildMVarName(fullName, "mvar_full_", instance);
    buildMVarName(pipeName, "mvar_pipe_", instance);

    int mutexId = (int)sys_sem_open(mutexName, 0);
    int emptyId = (int)sys_sem_open(emptyName, 0);
    int fullId = (int)sys_sem_open(fullName, 0);
    int pipeId = (int)sys_pipe_open(pipeName);

    if (mutexId < 0 || emptyId < 0 || fullId < 0 || pipeId < 0) {
        printColor("mvar reader: init error\n", RED);
        return;
    }

    while (1) {
        mvar_random_delay(writers + index + 1);
        sys_sem_wait((uint8_t)fullId);
        sys_sem_wait((uint8_t)mutexId);
        char value = 0;
        sys_pipe_read((unsigned int)pipeId, &value, 1);
        sys_sem_post((uint8_t)mutexId);
        sys_sem_post((uint8_t)emptyId);

        uint64_t color = mvarColors[index % mvarColorCount];
        printColorChar(value, color);
    }
}

void mvar(uint8_t argc, char **argv) {
    if (argc < 3) {
        printColor("Usage: mvar <writers> <readers>\n", RED);
        return;
    }
    if (!isNumber(argv[1]) || !isNumber(argv[2])) {
        printColor("mvar: writers and readers must be numeric.\n", RED);
        return;
    }

    int writers = (int)atoi(argv[1]);
    int readers = (int)atoi(argv[2]);

    if (writers <= 0 || readers <= 0) {
        printColor("mvar: writers and readers must be positive.\n", RED);
        return;
    }
    if (writers > MVAR_MAX_PARTICIPANTS) {
        printColor("mvar: writers out of range (max 10).\n", RED);
        return;
    }
    if (readers > MVAR_MAX_PARTICIPANTS) {
        printColor("mvar: readers out of range (max 10).\n", RED);
        return;
    }

    currentMVar.instanceId = ++mvarInstanceCounter;
    currentMVar.writerCount = writers;
    currentMVar.readerCount = readers;

    intToStr(currentMVar.instanceId, currentMVar.instanceStr);
    intToStr(writers, currentMVar.writerCountStr);
    intToStr(readers, currentMVar.readerCountStr);

    char emptyName[32] = {0};
    char fullName[32] = {0};
    char mutexName[32] = {0};
    char pipeName[32] = {0};

    buildMVarName(emptyName, "mvar_empty_", currentMVar.instanceId);
    buildMVarName(fullName, "mvar_full_", currentMVar.instanceId);
    buildMVarName(mutexName, "mvar_mutex_", currentMVar.instanceId);
    buildMVarName(pipeName, "mvar_pipe_", currentMVar.instanceId);

    int mutexId = (int)sys_sem_open(mutexName, 1);
    int emptyId = (int)sys_sem_open(emptyName, 1);
    int fullId = (int)sys_sem_open(fullName, 0);
    int pipeId = (int)sys_pipe_open(pipeName);

    if (mutexId < 0 || emptyId < 0 || fullId < 0 || pipeId < 0) {
        printColor("mvar: could not create synchronization primitives.\n", RED);
        return;
    }

    /* uint8_t turnIds[MVAR_MAX_PARTICIPANTS] = {0};
    for (int i = 0; i < writers; i++) {
        char turnName[32] = {0};
        buildMVarIndexedName(turnName, "mvar_turn_", currentMVar.instanceId, i);
        uint64_t id = sys_sem_open(turnName, 0);
        if (id == (uint64_t)-1) {
            printColor("mvar: could not create writer turn semaphores.\n", RED);
            return;
        }
        turnIds[i] = (uint8_t)id;
    } */

    for (int i = 0; i < writers; i++) {
        intToStr(i, currentMVar.writerIndexStr[i]);
        currentMVar.writerArgv[i][0] = buildWriterName(i+1);
        currentMVar.writerArgv[i][1] = currentMVar.writerIndexStr[i];
        currentMVar.writerArgv[i][2] = currentMVar.instanceStr;
        currentMVar.writerArgv[i][3] = currentMVar.writerCountStr;
        currentMVar.writerArgv[i][4] = NULL;
        sys_launch_process((void *)mvar_writer, 2, 4, currentMVar.writerArgv[i]);
    }
    /* if (writers > 0) {
        sys_sem_post(turnIds[0]);
    } */

    for (int i = 0; i < readers; i++) {
        intToStr(i, currentMVar.readerIndexStr[i]);
        currentMVar.readerArgv[i][0] = buildReaderName(i+1);
        currentMVar.readerArgv[i][1] = currentMVar.readerIndexStr[i];
        currentMVar.readerArgv[i][2] = currentMVar.instanceStr;
        currentMVar.readerArgv[i][3] = currentMVar.writerCountStr;
        currentMVar.readerArgv[i][4] = NULL;
        sys_launch_process((void *)mvar_reader, 2, 4 , currentMVar.readerArgv[i]);
    }
}