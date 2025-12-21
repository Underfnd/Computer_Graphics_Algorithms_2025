
/* 01_vertex_program.c - OpenGL-based very simple vertex program example
   using Cg program from Chapter 2 of "The Cg Tutorial" (Addison-Wesley,
   ISBN 0321194969). */

#include <windows.h>
#include <stdio.h>
#include <d3d9.h>     /* Can't include this?  Is DirectX SDK installed? */
#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgD3D9.h>

#include "DXUT.h"  /* DirectX Utility Toolkit (part of the DirectX SDK) */

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d9.lib")

static CGcontext myCgContext;
static CGprofile myCgVertexProfile;
static CGprogram myCgVertexProgram;

static LPDIRECT3DVERTEXBUFFER9 myVertexBuffer = NULL;

static const WCHAR *myProgramNameW = L"01_vertex_program";
static const char  *myProgramName = "01_vertex_program",
                   *myVertexProgramFileName = "C2E1v_green.cg",
/* Page 38 */      *myVertexProgramName = "C2E1v_green";

static int currentMode = 0;

static void checkForCgError(const char *situation)
{
  char buffer[4096];
  CGerror error;
  const char *string = cgGetLastErrorString(&error);
  
  if (error != CG_NO_ERROR) {
    if (error == CG_COMPILER_ERROR) {
      sprintf(buffer,
              "Program: %s\n"
              "Situation: %s\n"
              "Error: %s\n\n"
              "Cg compiler output...\n",
              myProgramName, situation, string);
      OutputDebugStringA(buffer);
      OutputDebugStringA(cgGetLastListing(myCgContext));
      sprintf(buffer,
              "Program: %s\n"
              "Situation: %s\n"
              "Error: %s\n\n"
              "Check debug output for Cg compiler output...",
              myProgramName, situation, string);
      MessageBoxA(0, buffer,
                  "Cg compilation error", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    } else {
      sprintf(buffer,
              "Program: %s\n"
              "Situation: %s\n"
              "Error: %s",
              myProgramName, situation, string);
      MessageBoxA(0, buffer,
                  "Cg runtime error", MB_OK | MB_ICONSTOP | MB_TASKMODAL);
    }
    exit(1);
  }
}

/* Forward declared DXUT callbacks registered by WinMain. */
static HRESULT CALLBACK OnResetDevice(IDirect3DDevice9*, const D3DSURFACE_DESC*, void*);
static void CALLBACK OnFrameRender(IDirect3DDevice9*, double, float, void*);
static void CALLBACK OnLostDevice(void*);
static void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  myCgContext = cgCreateContext();
  checkForCgError("creating context");
  cgSetParameterSettingMode(myCgContext, CG_DEFERRED_PARAMETER_SETTING);

  DXUTSetCallbackDeviceReset(OnResetDevice);
  DXUTSetCallbackDeviceLost(OnLostDevice);
  DXUTSetCallbackFrameRender(OnFrameRender);
  DXUTSetCallbackKeyboard(OnKeyboard);

  /* Parse  command line, handle  default hotkeys, and show messages. */
  DXUTInit();

  DXUTCreateWindow(myProgramNameW);

  /* Display 400x400 window. */
  DXUTCreateDevice(D3DADAPTER_DEFAULT, true, 400, 400);

  DXUTMainLoop();

  cgDestroyProgram(myCgVertexProgram);
  checkForCgError("destroying vertex program");
  cgDestroyContext(myCgContext);

  return DXUTGetExitCode();
}

static void createCgPrograms()
{
  const char **profileOpts;

  /* Determine the best profile once a device to be set. */
  myCgVertexProfile = cgD3D9GetLatestVertexProfile();
  checkForCgError("getting latest profile");

  profileOpts = cgD3D9GetOptimalOptions(myCgVertexProfile);
  checkForCgError("getting latest profile options");

  myCgVertexProgram =
    cgCreateProgramFromFile(
      myCgContext,              /* Cg runtime context */
      CG_SOURCE,                /* Program in human-readable form */
      myVertexProgramFileName,  /* Name of file containing program */
      myCgVertexProfile,        /* Profile: OpenGL ARB vertex program */
      myVertexProgramName,      /* Entry function name */
      profileOpts);             /* Pass optimal compiler options */
  checkForCgError("creating vertex program from file");
}

struct MY_V3F {
  FLOAT x, y, z;
};

static HRESULT initVertexBuffer(IDirect3DDevice9* pDev)
{
    /* Initialize vertices for rendering a 6-ray star. */
    static const MY_V3F starVertices[] = {
        // star's center (vertical 0)
        { 0.0f, 0.0f, 0.0f },

        // star's verticals
        { 0.0f, 0.8f, 0.0f },   // top
        { 0.7f, 0.4f, 0.0f },   // right top
        { 0.4f, -0.7f, 0.0f },  // right bottom
        { -0.4f, -0.7f, 0.0f }, // left bottom
        { -0.7f, 0.4f, 0.0f },  // left top
        { 0.0f, 0.8f, 0.0f }    // againt top
    };

    if (FAILED(pDev->CreateVertexBuffer(sizeof(starVertices),
        0, D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &myVertexBuffer, NULL))) {
        return E_FAIL;
    }

    void* pVertices;
    if (FAILED(myVertexBuffer->Lock(0, 0, /* map entire buffer */
        &pVertices, 0))) {
        return E_FAIL;
    }
    memcpy(pVertices, starVertices, sizeof(starVertices));
    myVertexBuffer->Unlock();

    return S_OK;
}

static HRESULT CALLBACK OnResetDevice(IDirect3DDevice9* pDev, 
                                      const D3DSURFACE_DESC* backBuf,
                                      void* userContext)
{
  cgD3D9SetDevice(pDev);
  checkForCgError("setting Direct3D device");

  static int firstTime = 1;
  if (firstTime) {
    /* Cg runtime resources such as CGprogram and CGparameter handles
       survive a device reset so we just need to compile a Cg program
       just once.  We do however need to unload Cg programs with
       cgD3DUnloadProgram upon when a Direct3D device is lost and load
       Cg programs every Direct3D device reset with cgD3D9UnloadProgram. */
    createCgPrograms();
    firstTime = 0;
  }

  /* false below means "no parameter shadowing" */
  cgD3D9LoadProgram(myCgVertexProgram, false, 0);
  checkForCgError("loading vertex program");

  return initVertexBuffer(pDev);
}

static void CALLBACK OnFrameRender(IDirect3DDevice9* pDev,
    double time,
    float elapsedTime,
    void* userContext)
{
    pDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f), 1.0f, 0); // background color

    if (SUCCEEDED(pDev->BeginScene())) {
        cgD3D9BindProgram(myCgVertexProgram);
        checkForCgError("binding vertex program");

        /* Render the star. */
        pDev->SetStreamSource(0, myVertexBuffer, 0, sizeof(MY_V3F));
        pDev->SetFVF(D3DFVF_XYZ);

        if (currentMode == 0)
        {
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 3); // Triangle Fan
        }
        else if (currentMode == 1)
        {
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 4); // Triangle Fan
        }
        else if (currentMode == 2)
        {
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 5); // Triangle Fan
        }
        else if (currentMode == 3)
        {
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 6); // Triangle Fan
        }
        else if (currentMode == 4)
        {
            pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 5); // Triangle Strip
        }
        else if (currentMode == 5)
        {
            pDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6); // Triangle List
        }

        pDev->EndScene();
    }
}

static void CALLBACK OnLostDevice(void* userContext)
{
  myVertexBuffer->Release();
  cgD3D9SetDevice(NULL);
}

static void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
    if (!bKeyDown) return; // handle only click on buttons

    switch (nChar) {
    case VK_ESCAPE:  // Esc
        PostQuitMessage(0);
        break;
    case '1':
        currentMode = 0;
        break;
    case '2':
        currentMode = 1;
        break;
    case '3':
        currentMode = 2;
        break;
    case VK_SPACE:
        currentMode = (currentMode + 1) % 3;
        break;
    }
}