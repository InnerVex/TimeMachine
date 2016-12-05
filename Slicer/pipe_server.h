#pragma once

class Pipe_Server
{
public:
    static int createPipe();
    static int writeToPipe();
    static int readFromPipe();
    static int usePipe();
};
