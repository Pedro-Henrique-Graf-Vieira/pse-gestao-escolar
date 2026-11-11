#include "pse.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Sistema sistema;
    if (!carregar_dados(&sistema)) {
        fputs("Não foi possível carregar os dados.\n", stderr);
        return 1;
    }

    executar_menu(&sistema);
    return 0;
}
