/*
Resources 
http://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
https://learn.microsoft.com/en-us/windows/win32/memory/reserving-and-committing-memory
https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-memory_basic_information
ChatGPT
*/
#include <stdio.h>
#include <windows.h>

//.4 Reports the State in user-friendly terms
void memoryState(LPVOID address) {
    MEMORY_BASIC_INFORMATION mbi;
    
    SIZE_T result = VirtualQuery(//Retrieves information about a range of pages in the virtual address space of the calling process.
        address, // lpAddress A pointer to the base address of the region of pages to be queried. 
        &mbi, // lpBuffer A pointer to a MEMORY_BASIC_INFORMATION move the address to the memory well allocated so we can get info on it
        sizeof(mbi) //dwLength The size of the buffer pointed to by the lpBuffer
    );
    
    if (result == 0) {//The return The return value is the actual number of bytes returned in the information buffer.
        printf("VirtualQuery failed.\n");
        return;
    }

    printf("State of Allocated Memory: ");
    if (mbi.State == MEM_COMMIT){
        printf("Committed\n");
        return;
    }
    if (mbi.State == MEM_RESERVE){
            printf("Reserved\n");
            return;
        }
    if (mbi.State == MEM_FREE){
            printf("Free\n");
            return;
        }
}

int main() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    //1.Queries the system to determine the page size
    printf("System Page Size: %lu bytes\n", sysInfo.dwPageSize); // 4096 bytes

    //2. Allocate 1MB of memory. The return value is the base address of the allocated region of pages.
    printf("Allocating %ld bytes (1MB) of memory.\n", sysInfo.dwPageSize*256);
    LPVOID allocatedMemory = VirtualAlloc( 
        NULL, // [in, option] lpadress = The starting address of the region to allocate.  
        sysInfo.dwPageSize*256, // [in] dwSize = The size of the region, in bytes.
        MEM_RESERVE | MEM_COMMIT, //[in] flAllocationType = The type of memory allocation. To reserve and commit pages in one step, call VirtualAlloc with MEM_COMMIT | MEM_RESERVE. or MEM_LARGE_PAGES use Allocates memory using large page support.
        PAGE_READWRITE //[in] flProtect = The memory protection for the region of pages to be allocated
    );


    //3. Queries the system to determine the state of the allocated memory
    memoryState(allocatedMemory);

    //5. De-allocates the memory
    printf("Deallocating memory.\n");
    VirtualFree(allocatedMemory, 0, MEM_RELEASE);

    // 6: Repeats the query to determine/report memory State
    memoryState(allocatedMemory);

    return 0;
}