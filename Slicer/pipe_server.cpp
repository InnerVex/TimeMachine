#include <pipe_server.h>
#include <iostream>
#include <windows.h>
#include <mutex>
#include <QDataStream>
#include <fstream>
#include <tchar.h>
#include <strsafe.h>
#include <QDebug>
//using namespace std;
std::mutex mut;

#define CONNECTING_STATE 0
#define READING_STATE 1
#define WRITING_STATE 2
#define INSTANCES 4
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

typedef struct
{
   OVERLAPPED oOverlap;
   HANDLE hPipeInst;
   TCHAR chRequest[BUFSIZE];
   DWORD cbRead;
   TCHAR chReply[BUFSIZE];
   DWORD cbToWrite;
   DWORD dwState;
   BOOL fPendingIO;
} PIPEINST, *LPPIPEINST;

VOID DisconnectAndReconnect(DWORD);
BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED);
VOID GetAnswerToRequest(LPPIPEINST);

PIPEINST Pipe[INSTANCES];
HANDLE hEvents[INSTANCES];

int Pipe_Server::createPipe()
{
    mut.lock();
    std::wcout << "Creating an instance of a named pipe..." << std::endl;
    mut.unlock();

    LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\my_pipe");

    // Create a pipe to send data
    HANDLE pipe = CreateNamedPipe(
                L"\\\\.\\pipe\\my_pipe", // name of the pipe
                PIPE_ACCESS_DUPLEX |
                FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_BYTE, // send data as a byte stream
                2, // only allow 1 instance of this pipe
                0, // no outbound buffer
                0, // no inbound buffer
                0, // use default wait time
                NULL // use default security attributes
                );

    if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
        std::wcout << "Failed to create pipe instance.";
        // look up error code here using GetLastError()
        system("pause");
        return 1;
    }


    mut.lock();
    std::wcout << "Waiting for a client to connect to the pipe..." << std::endl;
    mut.unlock();

    // This call blocks until a client process connects to the pipe
    BOOL result = ConnectNamedPipe(pipe, NULL);
    if (!result) {
        std::wcout << "Failed to make connection on named pipe." << std::endl;
        // look up error code here using GetLastError()
        CloseHandle(pipe); // close the pipe
        system("pause");
        return 1;
    }

    mut.lock();
    std::wcout << "Sending data to pipe..." << std::endl;
    mut.unlock();


    //std::cin>>pipe;
    //QDataStream newStream(pipe);





    OVERLAPPED oOverlap;

    HANDLE hEvents = CreateEvent(
                NULL,    // default security attribute
                TRUE,    // manual-reset event
                TRUE,    // initial state = signaled
                NULL);   // unnamed event object

    if (hEvents == NULL)
    {
        printf("CreateEvent failed with %d.\n", GetLastError());
        return 0;
    }

   oOverlap.hEvent = hEvents;


    // This call blocks until a client process reads all the data
    //const wchar_t *data = L"*** Hello Pipe World ***";
    DWORD numBytesWritten = 0;
    int k=0;
    while (k<3)
    {
        k++;
        //wchar_t *data = L"*** Hello Pipe World ***";
        const char *data = "Hi world";
        result = WriteFile(
                    pipe, // handle to our outbound pipe
                    data, // data to send
                    15,//strlen(data),//(wcslen(data) * sizeof(wchar_t), // length of data to send (bytes)
                    &numBytesWritten, // will store actual amount of data sent
                    &oOverlap // using overlapped IO
                    );

        std::fstream outfile("\\\\.\\pipe\\my_pipe");
        for(int j=0;j<10;j++)
            outfile<<(char)('0'+j);

        system("pause");


        mut.lock();
        std::wcout << "Number of bytes sent: " << numBytesWritten << std::endl;
        mut.unlock();

        /*if (result) {
            mut.lock();
            std::wcout << "Number of bytes sent: " << numBytesWritten << std::endl;
            mut.unlock();
        } else {
            mut.lock();
            std::wcout << "Failed to send data." << std::endl;
            mut.unlock();
            // look up error code here using GetLastError()
        }*/
    }




    system("pause");
    // Close the pipe (automatically disconnects client too)
    CloseHandle(pipe);

    mut.lock();
    std::wcout << "Server Done." << std::endl;
    mut.unlock();

    system("pause");
    return 0;
}

int Pipe_Server::writeToPipe()
{
    DWORD i, dwWait, cbRet, dwErr;
    BOOL fSuccess;
    LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

    // The initial loop creates several instances of a named pipe
    // along with an event object for each instance.  An
    // overlapped ConnectNamedPipe operation is started for
    // each instance.

    for (i = 0; i < INSTANCES; i++)
    {

        // Create an event object for this instance.

        hEvents[i] = CreateEvent(
                    NULL,    // default security attribute
                    TRUE,    // manual-reset event
                    TRUE,    // initial state = signaled
                    NULL);   // unnamed event object

        if (hEvents[i] == NULL)
        {
            printf("CreateEvent failed with %d.\n", GetLastError());
            return 0;
        }

        Pipe[i].oOverlap.hEvent = hEvents[i];

        Pipe[i].hPipeInst = CreateNamedPipe(
                    lpszPipename,            // pipe name
                    PIPE_ACCESS_DUPLEX |     // read/write access
                    FILE_FLAG_OVERLAPPED,    // overlapped mode
                    PIPE_TYPE_MESSAGE |      // message-type pipe
                    PIPE_READMODE_MESSAGE |  // message-read mode
                    PIPE_WAIT,               // blocking mode
                    INSTANCES,               // number of instances
                    BUFSIZE*sizeof(TCHAR),   // output buffer size
                    BUFSIZE*sizeof(TCHAR),   // input buffer size
                    PIPE_TIMEOUT,            // client time-out
                    NULL);                   // default security attributes

        if (Pipe[i].hPipeInst == INVALID_HANDLE_VALUE)
        {
            printf("CreateNamedPipe failed with %d.\n", GetLastError());
            return 0;
        }

        // Call the subroutine to connect to the new client

        Pipe[i].fPendingIO = ConnectToNewClient(
                    Pipe[i].hPipeInst,
                    &Pipe[i].oOverlap);

        Pipe[i].dwState = Pipe[i].fPendingIO ?
                    CONNECTING_STATE : // still connecting
                    READING_STATE;     // ready to read
    }


    while (1)
    {
        // Wait for the event object to be signaled, indicating
        // completion of an overlapped read, write, or
        // connect operation.

        dwWait = WaitForMultipleObjects(
                    INSTANCES,    // number of event objects
                    hEvents,      // array of event objects
                    FALSE,        // does not wait for all
                    INFINITE);    // waits indefinitely

        // dwWait shows which pipe completed the operation.

        i = dwWait - WAIT_OBJECT_0;  // determines which pipe
        if (i < 0 || i > (INSTANCES - 1))
        {
            printf("Index out of range.\n");
            return 0;
        }

        // Get the result if the operation was pending.

        if (Pipe[i].fPendingIO)
        {
            fSuccess = GetOverlappedResult(
                        Pipe[i].hPipeInst, // handle to pipe
                        &Pipe[i].oOverlap, // OVERLAPPED structure
                        &cbRet,            // bytes transferred
                        FALSE);            // do not wait

            switch (Pipe[i].dwState)
            {
            // Pending connect operation
            case CONNECTING_STATE:
                if (! fSuccess)
                {
                    printf("Error %d.\n", GetLastError());
                    return 0;
                }
                Pipe[i].dwState = READING_STATE;
                break;

                // Pending read operation
            case READING_STATE:
                if (! fSuccess || cbRet == 0)
                {
                    DisconnectAndReconnect(i);
                    continue;
                }
                Pipe[i].cbRead = cbRet;
                Pipe[i].dwState = WRITING_STATE;
                break;

                // Pending write operation
            case WRITING_STATE:
                if (! fSuccess || cbRet != Pipe[i].cbToWrite)
                {
                    DisconnectAndReconnect(i);
                    continue;
                }
                Pipe[i].dwState = READING_STATE;
                break;

            default:
            {
                printf("Invalid pipe state.\n");
                return 0;
            }
            }
        }

        // The pipe state determines which operation to do next.

        switch (Pipe[i].dwState)
        {
        // READING_STATE:
        // The pipe instance is connected to the client
        // and is ready to read a request from the client.

        case READING_STATE:
            fSuccess = ReadFile(
                        Pipe[i].hPipeInst,
                        Pipe[i].chRequest,
                        BUFSIZE*sizeof(TCHAR),
                        &Pipe[i].cbRead,
                        &Pipe[i].oOverlap);

            // The read operation completed successfully.

            if (fSuccess && Pipe[i].cbRead != 0)
            {
                Pipe[i].fPendingIO = FALSE;
                Pipe[i].dwState = WRITING_STATE;
                continue;
            }

            // The read operation is still pending.

            dwErr = GetLastError();
            if (! fSuccess && (dwErr == ERROR_IO_PENDING))
            {
                Pipe[i].fPendingIO = TRUE;
                continue;
            }

            // An error occurred; disconnect from the client.

            DisconnectAndReconnect(i);
            break;

            // WRITING_STATE:
            // The request was successfully read from the client.
            // Get the reply data and write it to the client.

        case WRITING_STATE:
            GetAnswerToRequest(&Pipe[i]);

            fSuccess = WriteFile(
                        Pipe[i].hPipeInst,
                        Pipe[i].chReply,
                        Pipe[i].cbToWrite,
                        &cbRet,
                        &Pipe[i].oOverlap);

            // The write operation completed successfully.

            if (fSuccess && cbRet == Pipe[i].cbToWrite)
            {
                Pipe[i].fPendingIO = FALSE;
                Pipe[i].dwState = READING_STATE;
                continue;
            }

            // The write operation is still pending.

            dwErr = GetLastError();
            if (! fSuccess && (dwErr == ERROR_IO_PENDING))
            {
                Pipe[i].fPendingIO = TRUE;
                continue;
            }

            // An error occurred; disconnect from the client.

            DisconnectAndReconnect(i);
            break;

        default:
        {
            printf("Invalid pipe state.\n");
            return 0;
        }
        }
    }


    mut.lock();
    std::wcout << "Writing Done." << std::endl;
    mut.unlock();

    system("pause");
    return 0;
}

int Pipe_Server::readFromPipe()
{
    mut.lock();
    std::wcout << "Connecting to pipe..." << std::endl;
    mut.unlock();


    // Open the named pipe
    // Most of these parameters aren't very relevant for pipes.
    HANDLE pipe = CreateFile(
        L"\\\\.\\pipe\\my_pipe",
        GENERIC_READ, // only need read access
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        mut.lock();
        std::wcout << "Failed to connect to pipe." << std::endl;
        mut.unlock();
        // look up error code here using GetLastError()
        system("pause");
        return 1;
    }


    mut.lock();
    std::wcout << "Reading data from pipe..." << std::endl;
    mut.unlock();

    OVERLAPPED oOverlap;

    HANDLE hEvents = CreateEvent(
                NULL,    // default security attribute
                TRUE,    // manual-reset event
                TRUE,    // initial state = signaled
                NULL);   // unnamed event object

    if (hEvents == NULL)
    {
        printf("CreateEvent failed with %d.\n", GetLastError());
        return 0;
    }

   oOverlap.hEvent = hEvents;

    // The read operation will block until there is data to read
   //wchar_t buffer[128];
   int limit = 128;
   char buffer[limit];
   DWORD numBytesRead = 0;
   BOOL result = true;
   while(result)
   {
       result = ReadFile(
                   pipe,
                   buffer, // the data from the pipe will be put here
                   limit,//127 * sizeof(wchar_t), // number of bytes allocated
                   &numBytesRead, // this will store number of bytes actually read
                   &oOverlap // not using overlapped IO
                   );

        mut.lock();
        buffer[numBytesRead] = '\0'; // null terminate the string
        //buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
        std::wcout << "Number of bytes read: " << numBytesRead << std::endl;
        std::wcout << "Message: " << buffer << std::endl;
        mut.unlock();

        /*if (result) {
            mut.lock();
            buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
            std::wcout << "Number of bytes read: " << numBytesRead << std::endl;
            std::wcout << "Message: " << buffer << std::endl;
            mut.unlock();
        } else {
            mut.lock();
            std::wcout << "Failed to read data from the pipe." << std::endl;
            mut.unlock();
        }*/
    }

    // Close our pipe handle
    CloseHandle(pipe);

    mut.lock();
    std::wcout << "Reading Done." << std::endl;
    mut.unlock();

    system("pause");
    return 0;
}

int Pipe_Server::usePipe()
{
    mut.lock();
    std::wcout << "Connecting to pipe..." << std::endl;
    mut.unlock();

    // Open the named pipe
    // Most of these parameters aren't very relevant for pipes.
    HANDLE pipe = CreateFile(
        L"\\\\.\\pipe\\my_pipe",
        GENERIC_READ, // only need read access
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        mut.lock();
        std::wcout << "Failed to connect to pipe." << std::endl;
        mut.unlock();
        // look up error code here using GetLastError()
        system("pause");
        return 1;
    }

    mut.lock();
    std::wcout << "Reading data from pipe..." << std::endl;
    mut.unlock();

    // The read operation will block until there is data to read
    wchar_t buffer[128];
    DWORD numBytesRead = 0;
    BOOL result = true;
    while(result)
    {
        result = ReadFile(
                    pipe,
                    buffer, // the data from the pipe will be put here
                    127 * sizeof(wchar_t), // number of bytes allocated
                    &numBytesRead, // this will store number of bytes actually read
                    NULL // not using overlapped IO
                    );

        if (result) {
            mut.lock();
            buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
            std::wcout << "Number of bytes read: " << numBytesRead << std::endl;
            std::wcout << "Message: " << buffer << std::endl;
            mut.unlock();
        } else {
            mut.lock();
            std::wcout << "Failed to read data from the pipe." << std::endl;
            mut.unlock();
        }
    }

    // Close our pipe handle
    CloseHandle(pipe);

    mut.lock();
    std::wcout << "Client Done." << std::endl;
    mut.unlock();

    system("pause");
    return 0;
}

// DisconnectAndReconnect(DWORD)
// This function is called when an error occurs or when the client
// closes its handle to the pipe. Disconnect from this client, then
// call ConnectNamedPipe to wait for another client to connect.

VOID DisconnectAndReconnect(DWORD i)
{
// Disconnect the pipe instance.

   if (! DisconnectNamedPipe(Pipe[i].hPipeInst) )
   {
      printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
   }

// Call a subroutine to connect to the new client.

   Pipe[i].fPendingIO = ConnectToNewClient(
      Pipe[i].hPipeInst,
      &Pipe[i].oOverlap);

   Pipe[i].dwState = Pipe[i].fPendingIO ?
      CONNECTING_STATE : // still connecting
      READING_STATE;     // ready to read
}

// ConnectToNewClient(HANDLE, LPOVERLAPPED)
// This function is called to start an overlapped connect operation.
// It returns TRUE if an operation is pending or FALSE if the
// connection has been completed.

BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
   BOOL fConnected, fPendingIO = FALSE;

// Start an overlapped connection for this pipe instance.
   fConnected = ConnectNamedPipe(hPipe, lpo);

// Overlapped ConnectNamedPipe should return zero.
   if (fConnected)
   {
      printf("ConnectNamedPipe failed with %d.\n", GetLastError());
      return 0;
   }

   switch (GetLastError())
   {
   // The overlapped connection in progress.
      case ERROR_IO_PENDING:
         fPendingIO = TRUE;
         break;

   // Client is already connected, so signal an event.

      case ERROR_PIPE_CONNECTED:
         if (SetEvent(lpo->hEvent))
            break;

   // If an error occurs during the connect operation...
      default:
      {
         printf("ConnectNamedPipe failed with %d.\n", GetLastError());
         return 0;
      }
   }

   return fPendingIO;
}

VOID GetAnswerToRequest(LPPIPEINST pipe)
{
   qDebug()<<"Hi";
   //_tprintf( TEXT("[%d] %s\n"), pipe->hPipeInst, pipe->chRequest);
   StringCchCopy( pipe->chReply, BUFSIZE, TEXT("Default answer from server") );
   pipe->cbToWrite = (lstrlen(pipe->chReply)+1)*sizeof(TCHAR);
}
