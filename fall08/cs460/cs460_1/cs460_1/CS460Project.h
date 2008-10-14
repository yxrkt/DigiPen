// =============================================================================
#ifndef ENGINE_H
#define ENGINE_H
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

    void Run();

  private:
    void Initialize();
    void Update();
    void Exit();

    LRESULT WndProc( UINT msg, WPARAM wParam, LPARAM lParam );

    Graphics graphics;
};

#endif
