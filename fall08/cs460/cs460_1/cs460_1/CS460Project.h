// =============================================================================
#ifndef CS460PROJECT_H
#define CS460PROJECT_H
// =============================================================================

#include "WindowBase.h"
#include "Graphics.h"

// =============================================================================
// ! CS460 Framework engine
// =============================================================================
class CS460Project : WindowBase
{
  public:
    CS460Project( HINSTANCE hInstance );
    ~CS460Project();

    int Run();

  private:
    void Initialize();
    void Update();
    void Cleanup();

    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

  private:
    Graphics graphics;
};

#endif
