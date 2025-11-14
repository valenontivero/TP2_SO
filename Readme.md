# Kernel: main elements of an operating system

## How to build and run

x64BareBones is a basic setup to develop operating systems for the Intel 64 bits architecture.

The final goal of the project is to provide an entry point for a kernel and the possibility to load extra binary modules separated from the main kernel.

Environment setup:
1- Install the following packages before building the Toolchain and Kernel:

nasm qemu gcc make

2- Build the Toolchain

Execute the following commands on the x64BareBones project directory:

  user@linux:$ cd Toolchain
  user@linux:$ make all

3- Build the Kernel

From the x64BareBones project directory run:

  user@linux:$ make all

4- Run the kernel

From the x64BareBones project directory run:

  user@linux:$ ./run.sh

Another way is to compile and run all at once is using the docker.sh script. This will create a docker image with all the required dependencies and compile the kernel.
  
    user@linux:$ ./docker.sh

## Compile and run using buddy memory allocator

To use the buddy memory allocator instead of the default one, you need to compile the kernel with the BUDDY_ALLOCATOR flag set to 1. You can do this by running:

    user@linux:$ ./docker.sh buddy

## Windows

If you are using Windows, you can use WSL2 (Windows Subsystem for Linux) to run the above commands. Make sure to have WSL2 installed and set up with a Linux distribution of your choice, and to dos2unix all .sh files
  

Be aware that you may need to change your audio device on the run.sh script.

## Comands that you can run on the kernel terminal

### List of commands

**help**: gives you a list of all existent commands.

**time**: prints the time of the OS.

**date**: prints the date of the OS.

**registers**: print the state of the registers at the time.

**fillregs**: fill the registers with stepped values for testing.

**div0**: divide by zero to trigger exception

**invalidop**: trigger invalid operation code exception

**pong**: go to play the pong game.

**clear**: clears the OS screen.

**hello**: prints hello every 5 seconds.

**testprint**: tests that you can create a process and it can print on screen.

**testsem**: tests that a process can block itself with a semaphore and another process can unblock it

**testpipe**: tests that 2 processes can comunicate between pipes

**testpriority**: tests the priorities of 3 processes

**mem**: prints current memory usage.

**ps**: lists the active processes with their attributes.

**loop** <seconds>: prints the process ID periodically (default 1 second).

**kill** <pid>: terminates the process with the given PID.

**nice** <pid> <priority>: sets the process priority (0 is highest).

**block** <pid>: toggles the blocked state of the process.

**wc**: counts the number of input lines.

**filter**: removes vowels from the input stream.

**cat**: prints the stdin exactly as received.

**echo** <text>: prints the provided arguments to the standard output.

**mvar** <writers> <readers>: launches writers/readers synchronized through an mvar.

## Special features

### Background processes

You can set a process in backgriund adding an '&' at the end of the command. For example:

  loop 2 &

### IPC by pipes
You can create a pipe between two processes using the '|' character. For example:

  echo hello | filter  (this should print 'hll')
 
### Special inputs

You can press Ctrl+C to send a kill signal to the foreground process, and Ctrl+D to send an EOF signal.

### Example of combined features

Ypu can combine background processes and pipes. For example:

  loop 2 | filter &

This will launch a background process that prints the PID every 2 seconds, and pipes it to the filter process that removes vowels.

While this process is running in background, you can kill, block/unblock or change its priority from the terminal using its PID and the given commands. You can see all the running processes with the 'ps' command.


## Limitations and known issues

1. If you want to code your own processes that uses semaphores or pipes, be aware that you have to create/open/access them using a string identifier (name). 
 
2. The Mvar command does a heavy busy waiting, that may affect performance if many writers/readers are waiting on it.