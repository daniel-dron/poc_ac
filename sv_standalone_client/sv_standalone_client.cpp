#include <iostream>
#include <string>

#include "log.h"

#include "sv_service.h"

int main()
{
    sv_service sv{};
    sv.load();

    while (true)
    {
        sv.dispatch_kernel_detections();
        Sleep(10);
    }
}
