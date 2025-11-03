#include "LKCompiled.h"

////////////////////////////////////////////////////////////
bool fUse2D = true;
bool fUse3D = false;
bool fUseLogz = false;
LKDrawingGroup *fTop = nullptr, *fGroupFit = nullptr, *fGroupPID = nullptr;
LKDrawing *fDraw2D = nullptr, *fDraw3D = nullptr;
TObjArray *fHistDataArray = nullptr, *fHistFitGArray = nullptr, *fHistBackArray = nullptr, *fHistTestArray = nullptr, *fHistErrrArray = nullptr;
TObjArray* fFitArray = nullptr;
TObjArray* fContourGraphArray = nullptr;
vector<TString> fListGenFile;
TFile* fDataFile = nullptr;
TTree* fDataTree = nullptr;
TH2D *fHistPID = nullptr;
bool fRunCollected = false, fInitialized = false;
int fCurrentRunNumber, fCurrentType;
double fSigDist = 1;
int fNumContours = 10;
LKBinning fBnn2, fBnn3;
TString fCurrentFileName;
double fFinalContourAScale = 0.2;
TGraph* fFinalContourGraph = nullptr;
double fContourScaleList[] = {0.9,0.5};
TString fFormulaRotated2DGaussian = "[0]*exp(-0.5*(pow(((x-[1])*cos([5])+(y-[3])*sin([5]))/[2],2)+pow((-(x-[1])*sin([5])+(y-[3])*cos([5]))/[4],2)))";

////////////////////////////////////////////////////////////
void ResetBinning();
void SetBinningX(int nx, double x1, double x2, int fill=1);
void SetBinningY(int ny, double y1, double y2, int fill=1);
void SetBinningX(double x1, double x2, int fill=1);
void SetBinningY(double y1, double y2, int fill=1);
void SetBinning(int nx, double x1, double x2, int ny, double y1, double y2);
void SetBinning(double x1, double x2, double y1, double y2);
void SetBinWX(double w, int fill=1);
void SetBinWY(double w, int fill=1);
void SetBinNX(double n, int fill=1);
void SetBinNY(double n, int fill=1);
void SaveBinning();

////////////////////////////////////////////////////////////
void Init();
void Guide(TString mode);
bool ListFiles(bool newList=false, TString path="data", TString format="gen.root");
bool SelectFile(int idx=-1);
void CreateAndFillHistogram(int printb=1);
void DrawPoints(int redraw=0, vector<vector<double>> points=vector<vector<double>>{});
LKDrawing* GetFitTestDrawing(int i, TH2D *hist, TF2* fit, TF2* fitContanminent=(TF2*)nullptr);
void FitTotal();
void MakeSummary();
TF2* Fit2DGaussian(TH2D *hist, int idx, double valueX, double valueY, double sigmaX=2.5, double sigmaY=1.5, double theta=21);
TGraph *GetContourGraph(double contoA, double amplit, double valueX, double sigmaX, double valueY, double sigmaY, double thetaR);
double IntegralInsideGraph(TH2D* hist, TGraph* graph, bool justCount=true);
double IntegralInsideGraph(TH2D* hist, TGraph* graph, TF2 *f2, bool justCount=true);
double Integral2DGaussian(double amplitude, double sigma_x, double sigma_y, double contoA=0);
double Integral2DGaussian(TF2 *f2, double contoA=0);
void CollectRootFiles(std::vector<TString> &listGenFile, const char *dataPath="data", const TString &format="gen.root");
void ChangeSigDist(double sigDist=-1);
void SetContourAScale(double scale=-1);

////////////////////////////////////////////////////////////
#define _help         Guide("i");
#define _reset_bin    ResetBinning();
#define _save_bin     SaveBinning();
#define _set_contour  SetContourAScale(scale);
#define _list_files   ListFiles();
#define _select_files SelectFile();
#define _draw_points  DrawPoints();
#define _draw_pid     DrawPoints(2);
#define _select_pid   DrawPoints(1);
#define _fit_total    FitTotal();
#define _summary      MakeSummary();
#define _set_sigdist  ChangeSigDist();

#define ii Guide("i");
#define bb ResetBinning();
#define vv SaveBinning();
#define ee SetContourAScale(scale);
#define aa ListFiles();
#define ss SelectFile();
#define dd DrawPoints();
#define tt DrawPoints(2);
#define rr DrawPoints(1);
#define ff FitTotal();
#define gg MakeSummary();
#define qq ChangeSigDist();

#define e_title std::cout << "\033[0;35m" << "[RUN-" << fCurrentRunNumber << "] ============= "  << "\033[0m"
