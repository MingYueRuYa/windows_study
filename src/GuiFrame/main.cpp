#include "global.h"
#include "globalhead.h"

int main(char *argc[], int argv)
{
    QWinApp *app = GlobalGetApp();
    assert(app);

    app->InitInstance();
    app->run();
    app->ExitInstance();

    return 0;
}