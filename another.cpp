#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>

int main()
{
    const char *PATHNAME = "/tmp"; // Ensure this path exists and is the same in all files
    const int PROJ_ID = 'A';       // Same project identifier in all files

    key_t SHM_KEY = ftok(PATHNAME, PROJ_ID);
    if (SHM_KEY == -1)
    {
        perror("ftok failed");
        return 1;
    }

    std::cout << "Shared Memory Key: " << SHM_KEY << std::endl;
    return 0;
}
