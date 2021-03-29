#include "global.h"

QWinApp *g_pWinApp = nullptr;

QWinApp *GlobalGetApp()
{
    return g_pWinApp;
}