#include <iostream>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <wait.h>

int procInput(void) {//input from outside, s 2 10
    while (!std::cin.eof()) {
        // read a line of input until EOL
        std::string line;
        std::getline(std::cin, line);
        if (line.size()>0) {
            std::cout << line << std::endl;
        }
    }
    std::cout << "[B] saw EOF" << std::endl;
    return 0;
}

int main (int argc, char **argv) {
    std::vector<pid_t> kids;

    //create a pipe between rgen and A1
    int RtoA1[2];
    pipe(RtoA1);

    pid_t child_pid=fork();
    if(child_pid == 0)//rgen
    {
        //connect between rgen and A1 on the rgen side
        dup2(RtoA1[1], STDOUT_FILENO);
        close(RtoA1[0]);
        close(RtoA1[1]);

        int r = execv("./rgen",argv);
        return r;
    }
    else if (child_pid<0){
        std::cerr<<"Error: could not fork."<<std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    //create a pipe between A1 and A2
    int A1toA2[2];
    pipe(A1toA2);

    child_pid = fork();
    if(child_pid == 0)//a1
    {
        //connect between rgen and A1 on the A1 side
        dup2(RtoA1[0], STDIN_FILENO);
        close(RtoA1[0]);
        close(RtoA1[1]);

        //connect between A1 and A2 on the A1 side
        dup2(A1toA2[1], STDOUT_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);

        char *args2[] = {"python3", "./ece650-a1.py", NULL};
        return execvp("python3", args2);
    }
    else if (child_pid<0){
        std::cerr<<"Error: could not fork."<<std::endl;
        return 1;
    }
    kids.push_back(child_pid);


    child_pid = fork();
    if(child_pid == 0)
    {
        //connect between A1 and A2 on the A2 side
        dup2(A1toA2[0], STDIN_FILENO);
        close(A1toA2[0]);
        close(A1toA2[1]);

        int a2 = execv("./ece650-a2", argv);
        return a2;
    }
    else if (child_pid<0){
        std::cerr<<"Error: could not fork."<<std::endl;
        return 1;
    }
    kids.push_back(child_pid);

    //parent process
    // generate a pipe between keyboard input and A2 on the keyboard input side
    dup2(A1toA2[1], STDOUT_FILENO);
    close(A1toA2[0]);
    close(A1toA2[1]);
    int res = procInput();

    //send kill signals to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }

    return res;
}
