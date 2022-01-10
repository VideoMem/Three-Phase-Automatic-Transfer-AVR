# -*- coding: utf-8 -*-
import sys
import threading
from Manchester import Manchester

exit_flag = 0
man = Manchester()

def print_help():
    print("Console commands help:")
    print("")
    print("\thelp:", "displays this help")
    print("\tquit:", "exits application")
    print("")
    print("Device commands help:")
    print("")
    print("\tset parameter <value>:", "sets <value> for given parameter name")
    print("\tshow parameters:", "show parameters list and short description")
    print("")

def input_read(prompt):
    cmd = input(prompt)
    return cmd

def prompt_str():
    return ": "

def prompt():
    global exit_flag
    global man

    while not exit_flag:
        command = input(prompt_str())
        exec_command = True
        if command == "quit":
            sys.stdout.write("Waiting thread to finish\n" + prompt_str())
            exit_flag = True
        if command == "help":
            print_help()

        if exec_command:
            print(man.send(command))

def echo():
    global exit_flag
    global man
    newline = True

    while not exit_flag:
        chr = man.receive()

        if newline:
            sys.stdout.write("\r")
            newline = False

        if not exit_flag:
            sys.stdout.write(chr)

        if chr == "\n":
            newline = True
            sys.stdout.write(prompt_str())

        sys.stdout.flush()

class consoleThread(threading.Thread):
    def __init__(self, thread_id, name, callable):
        threading.Thread.__init__(self)
        self.threadID = thread_id
        self.name = name
        self.callable = callable
    def run(self):
        if self.callable == "prompt":
            prompt()
        if self.callable == "echo":
            echo()

threads = []

thread1 = consoleThread(1, "Thread-1", "echo")
thread2 = consoleThread(2, "Thread-2", "prompt")

# Start new Threads
thread1.start()
thread2.start()

# Add threads to thread list
threads.append(thread1)
threads.append(thread2)

# Wait for all threads to complete
for t in threads:
    t.join()

print("Bye!")
