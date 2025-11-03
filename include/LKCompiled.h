/// You may used this when you don't have LILAK.
/// This is dummy LKCompiled.h for the use of some of the tools in LILAK package.

#ifndef LILAK_VERSION

#define LILAK_VERSION "dummy"
#define LILAK_HASH "dummy"

#define LILAK_MAINPROJECT_VERSION "dummy"
#define LILAK_MAINPROJECT_HASH "dummy"

#define LILAK_HOSTNAME "dummy_host"
#define LILAK_USERNAME "dummy_user"
#define LILAK_HOSTUSER "dummy_host"
#define LILAK_PATH "dummy_path"

#define N_LILAK_PROJECT 0
#define LILAK_PROJECT_ALL "no_projects"
static const char* const LILAK_PROJECT_NAMES[] = {};
static const char* const LILAK_PROJECT_VERSIONS[] = {};
#define LILAK_PROJECT_NAME(i)    (LILAK_PROJECT_NAMES[(i)])
#define LILAK_PROJECT_VERSION(i) (LILAK_PROJECT_VERSIONS[(i)])

#include "LKCut.cpp"
#include "LKMisc.cpp"
#include "LKLogger.cpp"
#include "LKDrawing.cpp"
#include "LKPainter.cpp"
#include "LKBinning.cpp"
#include "LKBinning1.cpp"
#include "LKVector3.cpp"
#include "LKParameter.cpp"
#include "LKDataViewer.cpp"
#include "LKDrawingGroup.cpp"
#include "LKParameterContainer.cpp"

#endif
