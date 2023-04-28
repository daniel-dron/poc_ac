#include <iostream>
#include <string>

#include "log.h"

#include "sv_service.h"

int main()
{
    sv_service sv{};
    sv.load();

    sv.scan_system_threads();
}
