
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

//static LPDIRECT3DVERTEXBUFFER9 myVertexBuffer = NULL;
static LPDIRECT3DVERTEXBUFFER9 myVertexBufferFan = NULL;
static LPDIRECT3DVERTEXBUFFER9 myVertexBufferFan4Ray = NULL;
static LPDIRECT3DVERTEXBUFFER9 myVertexBufferFan6Ray = NULL;
static LPDIRECT3DVERTEXBUFFER9 myVertexBufferStrip = NULL;
static LPDIRECT3DVERTEXBUFFER9 myVertexBufferList = NULL;

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
    static const MY_V3F starVerticesFan[] = {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.8f, 0.0f },
        { 0.2f, 0.4f, 0.0f },

        { 0.7f, 0.4f, 0.0f },
        { 0.25f, -0.15f, 0.0f },
        { 0.4f, -0.7f, 0.0f },

        { 0.0f, -0.4f, 0.0f },
        { -0.4f, -0.7f, 0.0f },
        { -0.25f, -0.15f, 0.0f },

        { -0.7f, 0.4f, 0.0f },
        { -0.2f, 0.4f, 0.0f },
        { 0.0f, 0.8f, 0.0f },
    };

    static const MY_V3F starVerticesFan4Ray[] = {
        { 0.0f, 0.0f, 0.0f },

        { 0.0f, 0.8f, 0.0f },
        { 0.2f, 0.2f, 0.0f },

        { 0.8f, 0.0f, 0.0f },
        { 0.2f, -0.2f, 0.0f },

        { 0.0f, -0.8f, 0.0f },
        { -0.2f, -0.2f, 0.0f },

        { -0.8f, 0.0f, 0.0f },
        { -0.2f, 0.2f, 0.0f },

        { 0.0f, 0.8f, 0.0f },
    };

    static const MY_V3F starVerticesFan6Ray[] = {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.8f, 0.0f },
        { 0.4f, 0.4f, 0.0f },
        { 0.4f, -0.4f, 0.0f },
        { 0.4f, -0.4f, 0.0f },
        { 0.0f, -0.8f, 0.0f },
        { -0.4f, -0.4f, 0.0f },
        { -0.4f, 0.4f, 0.0f },
        { 0.0f, 0.8f, 0.0f },
    };

    static const MY_V3F starVerticesStrip[] = {
        { 0.0f, 0.0f, 0.0f },
        { -0.7f, 0.4f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { -0.2f, 0.4f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 0.8f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { 0.2f, 0.4f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { 0.7f, 0.4f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { 0.25f, -0.15f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { 0.4f, -0.7f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { 0.0f, -0.4f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { -0.4f, -0.7f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { -0.25f, -0.15f, 0.0f },

        { 0.0f, 0.0f, 0.0f },
        { -0.7f, 0.4f, 0.0f },
    };

    static const MY_V3F starVerticesList[] = {
        { 0.0f, 0.8f, 0.0f },
        { 0.2f, 0.4f, 0.0f },
        { -0.2f, 0.4f, 0.0f },

        { -0.7f, 0.4f, 0.0f },
        { -0.2f, 0.4f, 0.0f },
        { -0.25f, -0.15f, 0.0f },

        { -0.2f, 0.4f, 0.0f },
        { 0.2f, 0.4f, 0.0f },
        { -0.25f, -0.15f, 0.0f },

        { -0.25f, -0.15f, 0.0f },
        { 0.2f, 0.4f, 0.0f },
        { 0.25f, -0.15f, 0.0f },

        { 0.2f, 0.4f, 0.0f },
        { 0.7f, 0.4f, 0.0f },
        { 0.25f, -0.15f, 0.0f },

        { -0.25f, -0.15f, 0.0f },
        { 0.0f, -0.15f, 0.0f },
        { 0.0f, -0.4f, 0.0f },

        { 0.0f, -0.15f, 0.0f },
        { 0.25f, -0.15f, 0.0f },
        { 0.0f, -0.4f, 0.0f },

        { 0.0f, -0.4f, 0.0f },
        { 0.25f, -0.15f, 0.0f },
        { 0.4f, -0.7f, 0.0f },

        { 0.0f, -0.4f, 0.0f },
        { -0.4f, -0.7f, 0.0f },
        { -0.25f, -0.15f, 0.0f }
    };
    

    // created fan buffers
    if (FAILED(pDev->CreateVertexBuffer(sizeof(starVerticesFan),
        0, D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &myVertexBufferFan, NULL))) {
        return E_FAIL;
    }
    void* pVertices;
    if (FAILED(myVertexBufferFan->Lock(0, 0, &pVertices, 0))) {
        myVertexBufferFan->Release();
        myVertexBufferFan = NULL;
        return E_FAIL;
    }
    memcpy(pVertices, starVerticesFan, sizeof(starVerticesFan));
    myVertexBufferFan->Unlock();


    // created fan4 buffers
    if (FAILED(pDev->CreateVertexBuffer(sizeof(starVerticesFan4Ray),
        0, D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &myVertexBufferFan4Ray, NULL))) {
        return E_FAIL;
    }
    if (FAILED(myVertexBufferFan4Ray->Lock(0, 0, &pVertices, 0))) {
        myVertexBufferFan4Ray->Release();
        myVertexBufferFan4Ray = NULL;
        return E_FAIL;
    }
    memcpy(pVertices, starVerticesFan4Ray, sizeof(starVerticesFan4Ray));
    myVertexBufferFan4Ray->Unlock();

    // created fan6 buffers
    if (FAILED(pDev->CreateVertexBuffer(sizeof(starVerticesFan6Ray),
        0, D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &myVertexBufferFan6Ray, NULL))) {
        return E_FAIL;
    }
    if (FAILED(myVertexBufferFan6Ray->Lock(0, 0, &pVertices, 0))) {
        myVertexBufferFan6Ray->Release();
        myVertexBufferFan6Ray = NULL;
        return E_FAIL;
    }
    memcpy(pVertices, starVerticesFan6Ray, sizeof(starVerticesFan6Ray));
    myVertexBufferFan6Ray->Unlock();

    // created strip buffers
    if (FAILED(pDev->CreateVertexBuffer(sizeof(starVerticesStrip),
        0, D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &myVertexBufferStrip, NULL))) {
        
        if (myVertexBufferFan) {
            myVertexBufferFan->Release();
            myVertexBufferFan = NULL;
        }
        return E_FAIL;
    }
    if (FAILED(myVertexBufferStrip->Lock(0, 0, &pVertices, 0))) {
        myVertexBufferFan->Release();
        myVertexBufferStrip->Release();
        myVertexBufferFan = NULL;
        myVertexBufferStrip = NULL;
        return E_FAIL;
    }
    memcpy(pVertices, starVerticesStrip, sizeof(starVerticesStrip));
    myVertexBufferStrip->Unlock();

    // created list buffers
    if (FAILED(pDev->CreateVertexBuffer(sizeof(starVerticesList),
        0, D3DFVF_XYZ,
        D3DPOOL_DEFAULT,
        &myVertexBufferList, NULL))) {
        
        if (myVertexBufferFan) {
            myVertexBufferFan->Release();
            myVertexBufferFan = NULL;
        }
        if (myVertexBufferStrip) {
            myVertexBufferStrip->Release();
            myVertexBufferStrip = NULL;
        }
        return E_FAIL;
    }
    if (FAILED(myVertexBufferList->Lock(0, 0, &pVertices, 0))) {
        myVertexBufferFan->Release();
        myVertexBufferStrip->Release();
        myVertexBufferList->Release();
        myVertexBufferFan = NULL;
        myVertexBufferStrip = NULL;
        myVertexBufferList = NULL;
        return E_FAIL;
    }
    memcpy(pVertices, starVerticesList, sizeof(starVerticesList));
    myVertexBufferList->Unlock();

    return S_OK;
}


/* in OnResetDevice call creating all buffers */
static HRESULT CALLBACK OnResetDevice(IDirect3DDevice9* pDev,
    const D3DSURFACE_DESC* backBuf,
    void* userContext)
{
    cgD3D9SetDevice(pDev);
    checkForCgError("setting Direct3D device");

    static int firstTime = 1;
    if (firstTime) {
        createCgPrograms();
        firstTime = 0;
    }

    cgD3D9LoadProgram(myCgVertexProgram, false, 0);
    checkForCgError("loading vertex program");

    return initVertexBuffer(pDev);  // create all buffers
}

static void CALLBACK OnFrameRender(IDirect3DDevice9* pDev,
    double time,
    float elapsedTime,
    void* userContext)
{
    pDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f), 1.0f, 0);

    if (SUCCEEDED(pDev->BeginScene())) {
        cgD3D9BindProgram(myCgVertexProgram);
        checkForCgError("binding vertex program");

        if (currentMode == 0 && myVertexBufferFan)
        {
            pDev->SetStreamSource(0, myVertexBufferFan, 0, sizeof(MY_V3F));
            pDev->SetFVF(D3DFVF_XYZ);
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 10);
        }
        else if (currentMode == 1 && myVertexBufferStrip)
        {
            pDev->SetStreamSource(0, myVertexBufferStrip, 0, sizeof(MY_V3F));
            pDev->SetFVF(D3DFVF_XYZ);
            pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 20);
        }
        else if (currentMode == 2 && myVertexBufferList)
        {
            pDev->SetStreamSource(0, myVertexBufferList, 0, sizeof(MY_V3F));
            pDev->SetFVF(D3DFVF_XYZ);
            pDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);
        }
        else if (currentMode == 3 && myVertexBufferFan4Ray)
        {
            pDev->SetStreamSource(0, myVertexBufferFan4Ray, 0, sizeof(MY_V3F));
            pDev->SetFVF(D3DFVF_XYZ);
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 10);
        }
        else if (currentMode == 4 && myVertexBufferFan6Ray)
        {
            pDev->SetStreamSource(0, myVertexBufferFan6Ray, 0, sizeof(MY_V3F));
            pDev->SetFVF(D3DFVF_XYZ);
            pDev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 10);
        }

        pDev->EndScene();
    }
}

static void CALLBACK OnLostDevice(void* userContext)
{
    if (myVertexBufferFan) {
        myVertexBufferFan->Release();
        myVertexBufferFan = NULL;
    }
    if (myVertexBufferFan4Ray) {
        myVertexBufferFan4Ray->Release();
        myVertexBufferFan4Ray = NULL;
    }
    if (myVertexBufferFan6Ray) {
        myVertexBufferFan6Ray->Release();
        myVertexBufferFan6Ray = NULL;
    }
    if (myVertexBufferStrip) {
        myVertexBufferStrip->Release();
        myVertexBufferStrip = NULL;
    }
    if (myVertexBufferList) {
        myVertexBufferList->Release();
        myVertexBufferList = NULL;
    }

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
    case '4':
        currentMode = 3;
        break;
    case '5':
        currentMode = 4;
        break;
    case VK_SPACE:
        currentMode = (currentMode + 1) % 5;
        break;
    }
}