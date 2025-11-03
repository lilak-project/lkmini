#include "include/draw_beam_pid.h"

void draw_beam_pid()
{
    Init();
    ListFiles(true);

    if (1)
    {
        fBnn2.SetNMM(120,-946,-907, 120,109,147);
        fBnn3.SetNMM(120,-946,-907, 120,109,147);
        SelectFile(0);
        //DrawPoints(0,vector<vector<double>>{vector<double>{-927.581, 130.787},vector<double>{-934.872649, 125.220195}});
        DrawPoints(0,vector<vector<double>>{vector<double>{-927.581, 130.787},vector<double>{-934.872649, 125.220195},vector<double>{-927.581, 124}});
        FitTotal();
        MakeSummary();
    }

    if (1)
    {
        fBnn2.SetNMM(120,-1010,-970, 120,110,145);
        fBnn3.SetNMM(120,-1010,-970, 120,110,145);
        SelectFile(0);
        //DrawPoints(0,vector<vector<double>>{vector<double>{-980.443,115.734}, vector<double>{-990.202,117.449}, vector<double>{-998.737,119.335}, vector<double>{-998.737,119.335}, vector<double>{-990.242,124.766}, vector<double>{-990.242,131.911},});
        DrawPoints(0,vector<vector<double>>{vector<double>{-980.443,115.734}, vector<double>{-990.202,117.449}, vector<double>{-998.737,119.335}, vector<double>{-990.242,124.766}});
        //DrawPoints(0,vector<vector<double>>{vector<double>{-980.443,115.734}, vector<double>{-990.202,117.449}, vector<double>{-998.737,119.335}, vector<double>{-998.737,119.335}});
        FitTotal();
        MakeSummary();
    }
}

void Init()
{
    gStyle -> SetNumberContours(100);
    gStyle -> SetOptStat("e");
    ResetBinning();
    fFitArray = new TObjArray();
    fHistDataArray = new TObjArray();
    fHistFitGArray = new TObjArray();
    fHistBackArray = new TObjArray();
    fHistTestArray = new TObjArray();
    fHistErrrArray = new TObjArray();
    fTop = new LKDrawingGroup();
    fGroupFit = fTop -> CreateGroup(Form("event_fit_%04d",fCurrentRunNumber));
    fGroupPID = fTop -> CreateGroup(Form("event_pid_%04d",fCurrentRunNumber));
    fDraw2D = fGroupPID -> CreateDrawing(Form("draw_2d_%04d",fCurrentRunNumber));
    fDraw3D = fGroupPID -> CreateDrawing(Form("draw_3d_%04d",fCurrentRunNumber),fUse3D);
    fGroupPID -> SetCanvasSize(1+int(fUse3D),1,1);
    fDraw2D -> SetCanvasMargin(.11,.15,.11,0.08);
    fDraw3D -> SetCanvasMargin(.10,.10,.10,.10);
    if (fUseLogz) fDraw2D -> SetLogz();
}

void CollectRootFiles(std::vector<TString> &listGenFile, const char *dataPath, const TString &format)
{
    e_info << "Looking for data files(" << format << ") in " << dataPath << endl;
    void *dirp = gSystem->OpenDirectory(dataPath);
    if (!dirp) {
        e_warning << dataPath << " do not exist!" << endl;
        return;
    }

    const char *entry;
    while ((entry = gSystem->GetDirEntry(dirp))) {
        TString fileName = entry;
        if (fileName == "." || fileName == "..") continue;
        TString fullPath = TString(dataPath) + "/" + fileName;
        if (gSystem->AccessPathName(fullPath, kFileExists)) continue;
        if (gSystem->OpenDirectory(fullPath)) {
            CollectRootFiles(listGenFile,fullPath, format);
        } else {
            if (fileName.EndsWith(format))
                listGenFile.push_back(fullPath);
        }
    }
    gSystem->FreeDirectory(dirp);
}

bool ListFiles(bool newList, TString path, TString format)
{
    e_title << "ListFiles" << endl;
    if (!fRunCollected || newList) {
        CollectRootFiles(fListGenFile,path,format);
        fRunCollected = true;
    }

    if (fListGenFile.size()==0) {
        e_info << "End of file list" << endl;
        return false;
    }

    int count = 0;
    cout << endl;
    e_info << "List of files" << endl;
    for (auto fileName : fListGenFile) {
        if (count%4==0) cout << "--------" << endl;
        cout << "   " << left << setw(5) << count++ << " " << fileName << endl;
    }
    cout << "--------" << endl;
    cout << endl;
    Guide("s");

    return true;
}

void CreateAndFillHistogram(int printb)
{
    if (printb) {
        fBnn2.Print();
        fBnn3.Print();
    }
    if (fDataTree) {
        Long64_t entriesTotal;
        //if (fCurrentType==2) fHistPID = fBnn2.NewH2(Form("histPID_%04d",fCurrentRunNumber),"f2ssde:rf0");
        //if (fCurrentType==3) fHistPID = fBnn3.NewH2(Form("histPID_%04d",fCurrentRunNumber),"f3ssde:rf0");
        if (fCurrentType==2) fHistPID = fBnn2.NewH2(Form("histPID_%04d",fCurrentRunNumber),";rf0;f2ssde");
        if (fCurrentType==3) fHistPID = fBnn3.NewH2(Form("histPID_%04d",fCurrentRunNumber),";rf0;f3ssde");
        if (fCurrentType==2) entriesTotal = fDataTree->Draw(Form("f2ssde:rf0>>%s",fHistPID->GetName()),"","goff");
        if (fCurrentType==3) entriesTotal = fDataTree->Draw(Form("f3ssde:rf0>>%s",fHistPID->GetName()),"","goff");
        e_info << "Total entries =" << entriesTotal << " (hist)=" << fHistPID -> GetEntries() << endl;
        fHistPID -> GetXaxis() -> SetTitleOffset(1.2);
        fHistPID -> SetTitle(Form("RUN %04d",fCurrentRunNumber));
        fDraw2D -> Clear();
        fDraw2D -> Add(fHistPID, "colz");
        fDraw2D -> SetStatsFillStyle(3001);
        fDraw3D -> Clear();
        fDraw3D -> Add(fHistPID, "lego2");
    }
}

bool SelectFile(int index=-1)
{
    e_title << "SelectFile" << endl;
    while (true)
    {
        if (index<0) {
            e_note << "Enter index number from the file list (q:exit): ";
            TString inputString;
            cin >> inputString;
            inputString = inputString.Strip(TString::kBoth);
            if (inputString=="x"||inputString=="q") { cout << "quit" << endl; return false; }
            if (inputString.IsDigit()==false) {
                e_info << "Invalid input." << endl;
                continue;
            }
            index = inputString.Atoi();
            if (index<0||index>=fListGenFile.size()) {
                cout << "Index not in range." << endl;
                continue;
            }
        }
        fCurrentFileName = fListGenFile.at(index);
        TString runNumberString = TString(fCurrentFileName(fCurrentFileName.Index("chkf")+8,4));
        if (runNumberString.IsDigit()) fCurrentRunNumber = runNumberString.Atoi();
        else fCurrentRunNumber = 9999;
        fListGenFile.erase(fListGenFile.begin()+index);
        cout << "   " << fCurrentFileName << endl;
        cout << "   (this file will be removed from the list)" << endl;
        break;
    }

    if (fDataFile) fDataFile -> Close();
    fDataFile = new TFile(fCurrentFileName,"read");
    fDataTree = (TTree*) fDataFile -> Get("tree");
    if      (fCurrentFileName.Index("chkf2run")>=0) { fCurrentType = 2; }
    else if (fCurrentFileName.Index("chkf3run")>=0) { fCurrentType = 3; }

    if (!fInitialized)
    {
        fInitialized = true;
    }
    CreateAndFillHistogram();

    fGroupFit -> SetName(Form("event_fit_%04d",fCurrentRunNumber));
    fGroupPID -> SetName(Form("event_pid_%04d",fCurrentRunNumber));
    fDraw2D -> SetName(Form("draw_2d_%04d",fCurrentRunNumber));
    fDraw3D -> SetName(Form("draw_3d_%04d",fCurrentRunNumber));

    fGroupPID -> Draw();
    cout << endl;
    Guide("td");

    return true;
}

TF2* Fit2DGaussian(TH2D *hist, int idx, double valueX, double valueY, double sigmaX, double sigmaY, double theta)
{
    TF2 *fit = nullptr;
    if (fCurrentType==2) fit = new TF2(Form("fit_%04d_%d", fCurrentRunNumber, idx), fFormulaRotated2DGaussian, valueX-fSigDist*sigmaX,valueX+fSigDist*sigmaX, valueY-fSigDist*sigmaY,valueY+fSigDist*sigmaY);
    if (fCurrentType==3) fit = new TF2(Form("fit_%04d_%d", fCurrentRunNumber, idx), fFormulaRotated2DGaussian, valueX-fSigDist*sigmaX,valueX+fSigDist*sigmaX, valueY-fSigDist*sigmaY,valueY+fSigDist*sigmaY);
    double amplit = hist -> GetBinContent(hist->GetXaxis()->FindBin(valueX),hist->GetYaxis()->FindBin(valueY));
    fit -> SetParameter(0, amplit);
    fit -> SetParameter(1, valueX);
    fit -> SetParameter(3, valueY);
    fit -> SetParameter(2, sigmaX);
    fit -> SetParameter(4, sigmaY);
    fit -> SetParameter(5, theta*TMath::DegToRad());
    fit -> SetParLimits(0, amplit*0.5, amplit*2);
    fit -> SetParLimits(1, valueX-0.5*sigmaX, valueX+0.5*sigmaX);
    fit -> SetParLimits(3, valueY-0.5*sigmaY, valueY+0.5*sigmaY);
    fit -> SetParLimits(2, sigmaX*0.5, sigmaX*1.2);
    fit -> SetParLimits(4, sigmaY*0.5, sigmaY*1.2);
    fit -> SetParLimits(5, 0.*TMath::Pi(), 0.25*TMath::Pi());
    fit -> SetLineColor(kRed);
    hist -> Fit(fit,"QBR0");
    fit -> SetContour(3);
    cout << "   " << fit -> GetName() << ": " << std::left
        << setw(16) << Form("amp=%f,", fit->GetParameter(0))
        << setw(28) << Form("x=(%f, %f),", fit->GetParameter(1), fit->GetParameter(2))
        << setw(28) << Form("y=(%f, %f),", fit->GetParameter(3), fit->GetParameter(4))
        << setw(18) << Form("theta=%f,", fit->GetParameter(5)*TMath::RadToDeg())
        << " (0)=" << fit -> Eval(fit->GetParameter(1)+0*fit->GetParameter(2),fit->GetParameter(3))
        << " (3)=" << fit -> Eval(fit->GetParameter(1)+3*fit->GetParameter(2),fit->GetParameter(3))
        << endl;
    return fit;
}

void DrawPoints(int redraw, vector<vector<double>> points)
{
    e_title << "DrawPoints" << endl;
    if (redraw==2) {
        fDraw2D -> Clear("!main");
        fGroupPID -> Draw();
        Guide("tr");
        return;
    }

    if (redraw==1) {
        fDraw2D -> Clear("!main");
        fGroupPID -> Draw();
    }

    if (points.size()==0)
    {
        cout << endl;
        e_info << "Draw graph with the mouse pointer! Double click to end." << endl;
        auto graph2 = (TGraph*) fDraw2D -> GetCanvas() -> WaitPrimitive("Graph","PolyLine");
        auto graph = (TGraph*) graph2 -> Clone();
        graph -> SetMarkerStyle(20);
        fDraw2D -> Add(graph,"samep");
        auto numPoints = graph -> GetN();
        for (auto iPoint=0; iPoint<numPoints; ++iPoint)
        {
            double x = graph -> GetPointX(iPoint);
            double y = graph -> GetPointY(iPoint);
            cout << "   " << iPoint << " " << x << " " << y << endl;
            points.push_back(vector<double>{x,y});
        }
    }

    fGroupFit -> Clear();
    double wx = fHistPID -> GetXaxis() -> GetBinWidth(1);
    double wy = fHistPID -> GetYaxis() -> GetBinWidth(1);
    double binA = wx*wy;

    int count = 0;
    fFitArray -> Clear();
    for (auto point : points)
    {
        auto fit = Fit2DGaussian(fHistPID, count, point[0], point[1]);
        fFitArray -> Add(fit);
        if (fUse2D) {
            double xx1, xx2, yy1, yy2;
            fit -> GetParLimits(1,xx1,xx2);
            fit -> GetParLimits(3,yy1,yy2);
            auto graphCenterRange = new TGraph();
            graphCenterRange -> SetLineColor(kGreen);
            graphCenterRange -> SetLineStyle(9);
            graphCenterRange -> SetPoint(0,xx1,yy1);
            graphCenterRange -> SetPoint(1,xx2,yy1);
            graphCenterRange -> SetPoint(2,xx2,yy2);
            graphCenterRange -> SetPoint(3,xx1,yy2);
            graphCenterRange -> SetPoint(4,xx1,yy1);
            fit -> GetRange(xx1,yy1,xx2,yy2);
            auto graphFitRange = new TGraph();
            graphFitRange -> SetLineColor(kYellow);
            graphFitRange -> SetLineStyle(9);
            graphFitRange -> SetPoint(0,xx1,yy1);
            graphFitRange -> SetPoint(1,xx2,yy1);
            graphFitRange -> SetPoint(2,xx2,yy2);
            graphFitRange -> SetPoint(3,xx1,yy2);
            graphFitRange -> SetPoint(4,xx1,yy1);
            fit -> SetRange(xx1-(xx2-xx1),yy1-(yy2-yy1),xx2+(xx2-xx1),yy2+(yy2-yy1));
            fDraw2D -> Add(graphCenterRange,"samel");
            fDraw2D -> Add(graphFitRange,"samel");
        }
        if (fUse2D)
        {
            auto amplit = fit->GetParameter(0);
            auto valueX = fit->GetParameter(1);
            auto sigmaX = fit->GetParameter(2);
            auto valueY = fit->GetParameter(3);
            auto sigmaY = fit->GetParameter(4);
            auto thetaR = fit->GetParameter(5);
            for (double contourScale : fContourScaleList) {
                auto graphC = GetContourGraph(contourScale*amplit, amplit, valueX, sigmaX, valueY, sigmaY, thetaR);
                graphC -> SetLineColor(kRed);
                graphC -> SetLineStyle(9);
                fDraw2D -> Add(graphC,"samel");
            }
            auto graphC = GetContourGraph(fFinalContourAScale*amplit, amplit, valueX, sigmaX, valueY, sigmaY, thetaR);
            graphC -> SetLineColor(kRed);
            fDraw2D -> Add(graphC,"samel");
        }
        count++;
    }
    fGroupPID -> Draw();
    fGroupFit -> Draw();

    Guide("rqf");

    return;
}

LKDrawing* GetFitTestDrawing(int idx, TH2D *hist, TF2* fit, TF2* fitContanminent)
{
    gStyle->SetPaintTextFormat(".3f");
    auto amplit = fit->GetParameter(0);
    auto valueX = fit->GetParameter(1);
    auto sigmaX = fit->GetParameter(2);
    auto valueY = fit->GetParameter(3);
    auto sigmaY = fit->GetParameter(4);
    auto thetaR = fit->GetParameter(5);
    TString nameData = fit -> GetName(); nameData .ReplaceAll("fit_","histIntegralData_");
    TString nameFitG = fit -> GetName(); nameFitG .ReplaceAll("fit_","histIntegralFitG_");
    TString nameTest = fit -> GetName(); nameTest .ReplaceAll("fit_","histIntegralTest_");
    TString nameBack = fit -> GetName(); nameBack .ReplaceAll("fit_","histIntegralBack_");
    TString nameErrr = fit -> GetName(); nameErrr.ReplaceAll("fit_","histIntegralErrr_");
    TString title = Form("[RUN %04d] (%d) Count in contour;S = Contour amplitude [A];Count",fCurrentRunNumber,idx);
    TH1D *histData = (TH1D*) fHistDataArray -> At(idx);
    TH1D *histFitG = (TH1D*) fHistFitGArray -> At(idx);
    TH1D *histBack = (TH1D*) fHistBackArray -> At(idx);
    TH1D *histTest = (TH1D*) fHistTestArray -> At(idx);
    TH1D *histErrr = (TH1D*) fHistErrrArray -> At(idx);
    if (histData==nullptr)
    {
        gROOT -> cd();
        histData = new TH1D(nameData,title,fNumContours,0,1);
        histFitG = new TH1D(nameFitG,title,fNumContours,0,1);
        histBack = new TH1D(nameBack,title,fNumContours,0,1);
        histTest = new TH1D(nameTest,title,fNumContours,0,1);
        histErrr = new TH1D(nameErrr,title,fNumContours,0,1);
        fHistDataArray -> Add(histData);
        fHistFitGArray -> Add(histFitG);
        fHistBackArray -> Add(histBack);
        fHistTestArray -> Add(histTest);
        fHistErrrArray -> Add(histErrr);
        histData -> SetFillColor(19);
        histData -> SetLineWidth(2);
        histData -> SetLineColor(kBlack);
        histData -> GetXaxis() -> SetTitleOffset(1.2);
        histFitG -> SetLineColor(kRed);
        histFitG -> SetLineWidth(2);
        histFitG -> SetLineStyle(2);
        histBack -> SetLineColor(kBlue);
        histBack -> SetLineWidth(2);
        histBack -> SetLineStyle(1);
    }
    else {
        histData -> Reset("ICES");
        histFitG -> Reset("ICES");
        histBack -> Reset("ICES");
        histTest -> Reset("ICES");
        histErrr -> Reset("ICES");
        histData -> SetTitle(title);
        histFitG -> SetTitle(title);
        histBack -> SetTitle(title);
        histTest -> SetTitle(title);
        histErrr -> SetTitle(title);
        histData -> SetName(nameData);
        histFitG -> SetName(nameFitG);
        histBack -> SetName(nameBack);
        histTest -> SetName(nameTest);
        histErrr -> SetName(nameErrr);
    }
    auto draw = new LKDrawing();
    draw -> SetCanvasMargin(0.15,0.05,0.1,0.1);
    draw -> SetOptStat(0);
    draw -> SetAutoMax();
    draw -> SetCreateLegend();
    draw -> Add(histData,"hist","data");
    if (fitContanminent!=nullptr)
        draw -> Add(histBack,"same hist","contaminent");
    draw -> Add(histFitG,"same","fit");
    //draw -> Add(histTest,"same","test");
    //draw -> Add(histErrr,"same text",".");
    draw -> AddLegendLine(Form("A=%.2f",amplit));
    draw -> AddLegendLine(Form("x=%.2f",valueX));
    draw -> AddLegendLine(Form("#sigma_{x}=%.2f",sigmaX));
    draw -> AddLegendLine(Form("y=%.2f",valueY));
    draw -> AddLegendLine(Form("#sigma_{y}=%.2f",sigmaY));
    draw -> AddLegendLine(Form("#theta=%.1f deg.",thetaR*TMath::RadToDeg()));
    double wx = hist -> GetXaxis() -> GetBinWidth(1);
    double wy = hist -> GetYaxis() -> GetBinWidth(1);
    double binA = wx*wy;
    double dc = (1./fNumContours);
    //double countFullG = Integral2DGaussian(fit, 0) / binA;
    double countFullG = 1;
    for (double contourScale=0; contourScale<1; contourScale+=dc) {
        auto graphC = GetContourGraph(contourScale*amplit, amplit, valueX, sigmaX, valueY, sigmaY, thetaR);
        graphC -> SetName(Form("contourGraph_%.2f",contourScale));
        if (contourScale==fFinalContourAScale) fFinalContourGraph = graphC;
        double countFitG = Integral2DGaussian(fit, contourScale*amplit);
        countFitG = countFitG / binA;
        histFitG -> SetBinContent(histFitG->GetXaxis()->FindBin(contourScale+0.5*dc),countFitG/countFullG);
        double countTest = IntegralInsideGraph(hist, graphC, fit);
        countTest = countTest;
        histTest -> SetBinContent(histTest->GetXaxis()->FindBin(contourScale+0.5*dc),countTest/countFullG);
        if (fitContanminent!=nullptr) {
            double countBack = IntegralInsideGraph(hist, graphC, fitContanminent);
            countBack = countBack;
            histBack -> SetBinContent(histBack->GetXaxis()->FindBin(contourScale+0.5*dc),countBack/countFullG);
        }
        if (contourScale!=0) {
            double countHist = IntegralInsideGraph(hist, graphC);
            histData -> SetBinContent(histData->GetXaxis()->FindBin(contourScale+0.5*dc),countHist/countFullG);
            histErrr -> SetBinContent(histErrr->GetXaxis()->FindBin(contourScale+0.5*dc),abs(countHist-countFitG)/countFitG);
        }
    }
    return draw;
}

void FitTotal()
{
    e_title << "FitTotal" << endl;
    fDraw2D -> Clear("!main");
    fGroupFit -> Clear();
    auto numFits = fFitArray -> GetEntries();
    TString formulaTotal;
    double xx1=DBL_MAX, xx2=-DBL_MAX, yy1=DBL_MAX, yy2=-DBL_MAX;
    for (auto iFit=0; iFit<numFits; ++iFit)
    {
        TString formulaCurrent = fFormulaRotated2DGaussian.Data();
        formulaCurrent.ReplaceAll("[0]",Form("[%d]",0+iFit*6));
        formulaCurrent.ReplaceAll("[1]",Form("[%d]",1+iFit*6));
        formulaCurrent.ReplaceAll("[2]",Form("[%d]",2+iFit*6));
        formulaCurrent.ReplaceAll("[3]",Form("[%d]",3+iFit*6));
        formulaCurrent.ReplaceAll("[4]",Form("[%d]",4+iFit*6));
        formulaCurrent.ReplaceAll("[5]",Form("[%d]",5+iFit*6));
        if (iFit==0) formulaTotal = formulaCurrent;
        else formulaTotal = formulaTotal + " + " + formulaCurrent;
        auto fit = (TF2*) fFitArray -> At(iFit);
        auto amplit = fit->GetParameter(0);
        auto valueX = fit->GetParameter(1);
        auto sigmaX = fit->GetParameter(2);
        auto valueY = fit->GetParameter(3);
        auto sigmaY = fit->GetParameter(4);
        auto thetaR = fit->GetParameter(5);
        auto x1 = valueX-fSigDist*sigmaX; if (xx1>x1) xx1 = x1;
        auto x2 = valueX+fSigDist*sigmaX; if (xx2<x2) xx2 = x2;
        auto y1 = valueY-fSigDist*sigmaY; if (yy1>y1) yy1 = y1;
        auto y2 = valueY+fSigDist*sigmaY; if (yy2<y2) yy2 = y2;
    }
    auto fitContanminent = new TF2(Form("fitContanminent_%04d", fCurrentRunNumber), formulaTotal, xx1, xx2, yy1, yy2);
    auto fitTotal = new TF2(Form("fitTotal_%04d", fCurrentRunNumber), formulaTotal, xx1, xx2, yy1, yy2);
    fitTotal -> SetLineColor(kMagenta);
    fitTotal -> SetContour(3);
    for (auto iFit=0; iFit<numFits; ++iFit)
    {
        auto fit = (TF2*) fFitArray -> At(iFit);
        auto amplit = fit->GetParameter(0);
        auto valueX = fit->GetParameter(1);
        auto valueY = fit->GetParameter(2);
        auto sigmaX = fit->GetParameter(3);
        auto sigmaY = fit->GetParameter(4);
        auto thetaR = fit->GetParameter(5);
        fitTotal -> SetParameter(0+iFit*6, amplit);
        fitTotal -> SetParameter(1+iFit*6, valueX);
        fitTotal -> SetParameter(2+iFit*6, valueY);
        fitTotal -> SetParameter(3+iFit*6, sigmaX);
        fitTotal -> SetParameter(4+iFit*6, sigmaY);
        fitTotal -> SetParameter(5+iFit*6, thetaR);
        fitTotal -> SetParLimits(0+iFit*6, amplit*0.8, amplit*1.2);
        fitTotal -> SetParLimits(1+iFit*6, valueX-0.1*sigmaX, valueX+0.1*sigmaX);
        fitTotal -> SetParLimits(2+iFit*6, valueY-0.1*sigmaY, valueY+0.1*sigmaY);
        fitTotal -> SetParLimits(3+iFit*6, sigmaX*0.8, sigmaX*1.2);
        fitTotal -> SetParLimits(4+iFit*6, sigmaY*0.8, sigmaY*1.2);
        fitTotal -> SetParLimits(5+iFit*6, thetaR-0.1*TMath::Pi(), thetaR+0.1*TMath::Pi());
    }
    fHistPID -> Fit(fitTotal,"QBR0");
    auto legend = new TLegend();
    legend -> SetFillStyle(3001);
    legend -> SetMargin(0.1);
    for (auto iFit=0; iFit<numFits; ++iFit)
    {
        auto fit = (TF2*) fFitArray -> At(iFit);
        auto amplit = fitTotal->GetParameter(0+iFit*6);
        auto valueX = fitTotal->GetParameter(1+iFit*6);
        auto sigmaX = fitTotal->GetParameter(2+iFit*6);
        auto valueY = fitTotal->GetParameter(3+iFit*6);
        auto sigmaY = fitTotal->GetParameter(4+iFit*6);
        auto thetaR = fitTotal->GetParameter(5+iFit*6);
        fit -> SetParameter(0,amplit);
        fit -> SetParameter(1,valueX);
        fit -> SetParameter(2,sigmaX);
        fit -> SetParameter(3,valueY);
        fit -> SetParameter(4,sigmaY);
        fit -> SetParameter(5,thetaR);
        fit -> SetLineColor(kMagenta);
        cout << "   " << fit -> GetName() << ": " << std::left
            << setw(16) << Form("amp=%f,", amplit)
            << setw(28) << Form("x=(%f, %f),", valueX, sigmaX)
            << setw(28) << Form("y=(%f, %f),", valueY, sigmaY)
            << setw(18) << Form("theta=%f", thetaR*TMath::RadToDeg())
            << endl;
        for (double contourScale : fContourScaleList) {
            auto graphC = GetContourGraph(contourScale*amplit, amplit, valueX, sigmaX, valueY, sigmaY, thetaR);
            graphC -> SetLineColor(kRed);
            graphC -> SetLineStyle(9);
            fDraw2D -> Add(graphC,"samel");
        }
        auto graphC = GetContourGraph(fFinalContourAScale*amplit, amplit, valueX, sigmaX, valueY, sigmaY, thetaR);
        graphC -> SetLineColor(kRed);
        fDraw2D -> Add(graphC,"samel");
        auto text = new TText(valueX,valueY,Form("%d",iFit));
        text -> SetTextAlign(22);
        text -> SetTextSize(0.02);
        if (amplit<fHistPID->GetMaximum()*0.3)
            text -> SetTextColor(kGreen);
        fDraw2D -> Add(text,"same");
        for (auto iPar=0; iPar<fitTotal->GetNpar(); ++iPar)
            fitContanminent->SetParameter(iPar,fitTotal->GetParameter(iPar));
        fitContanminent->SetParameter(0+iFit*6,0);
        auto draw = GetFitTestDrawing(iFit,fHistPID,fit,fitContanminent);
        fGroupFit -> Add(draw);
        TH1D *histData = (TH1D*) fHistDataArray -> At(iFit);
        TH1D *histBack = (TH1D*) fHistBackArray -> At(iFit);
        auto bin = histData -> FindBin(fFinalContourAScale+0.5*(1./fNumContours));
        auto count = histData -> GetBinContent(bin);
        auto contamination = histBack -> GetBinContent(bin);
        auto corrected = count - contamination;
        //fDraw2D -> AddLegendLine(Form("%d) cc=%d",iFit,int(corrected)));
        legend -> AddEntry((TObject*)nullptr,Form("[%d] %d (%d)",iFit,int(count),int(contamination)),"");
    }
    if (fUse2D) {
        auto graphFitRange = new TGraph();
        graphFitRange -> SetLineColor(kYellow);
        graphFitRange -> SetLineStyle(2);
        graphFitRange -> SetPoint(0,xx1,yy1);
        graphFitRange -> SetPoint(1,xx2,yy1);
        graphFitRange -> SetPoint(2,xx2,yy2);
        graphFitRange -> SetPoint(3,xx1,yy2);
        graphFitRange -> SetPoint(4,xx1,yy1);
        fDraw2D -> Add(graphFitRange,"samel");
    }
    //fDraw2D -> SetCreateLegend();
    fDraw2D -> SetLegendCorner(1);
    fDraw2D -> Add(legend);
    fGroupFit -> Draw();
    fGroupPID -> Draw();

    Guide("rqg");
}

TGraph *GetContourGraph(double contoA, double amplit, double valueX, double sigmaX, double valueY, double sigmaY, double thetaR)
{
    double Rx = sigmaX * sqrt(-2 * log(contoA / amplit));
    double Ry = sigmaY * sqrt(-2 * log(contoA / amplit));

    auto graph = new TGraph();
    graph -> SetMarkerStyle(20);

    for (double theta=0; theta<=2*TMath::Pi(); theta+=0.1) {
        double pointX = valueX + Rx * cos(theta);
        double pointY = valueY + Ry * sin(theta);
        TVector3 point(pointX-valueX,pointY-valueY,0);
        point.RotateZ(thetaR);
        graph -> SetPoint(graph->GetN(),point.X()+valueX,point.Y()+valueY);
    }
    graph -> SetPoint(graph->GetN(),graph->GetPointX(0),graph->GetPointY(0));
    return graph;
}

double IntegralInsideGraph(TH2D* hist, TGraph* graph, bool justCount)
{
    double integral = 0;
    int nx = hist -> GetXaxis() -> GetNbins();
    int ny = hist -> GetYaxis() -> GetNbins();
    double wx = hist -> GetXaxis() -> GetBinWidth(1);
    double wy = hist -> GetYaxis() -> GetBinWidth(1);
    double binA = wx*wy;
    if (justCount) binA = 1;
    for (auto xbin=1; xbin<=nx; ++xbin) {
        double xvalue = hist -> GetXaxis() -> GetBinCenter(xbin);
        for (auto ybin=1; ybin<=ny; ++ybin) {
            double yvalue = hist -> GetYaxis() -> GetBinCenter(ybin);
            if (graph -> IsInside(xvalue,yvalue)) {
                double value = hist -> GetBinContent(xbin,ybin);
                integral += value*binA;
            }
        }
    }
    return integral;
}

double IntegralInsideGraph(TH2D* hist, TGraph* graph, TF2 *f2, bool justCount)
{
    double integral = 0;
    int nx = hist -> GetXaxis() -> GetNbins();
    int ny = hist -> GetYaxis() -> GetNbins();
    double wx = hist -> GetXaxis() -> GetBinWidth(1);
    double wy = hist -> GetYaxis() -> GetBinWidth(1);
    double binA = wx*wy;
    if (justCount) binA = 1;
    for (auto xbin=1; xbin<=nx; ++xbin) {
        double xvalue = hist -> GetXaxis() -> GetBinCenter(xbin);
        for (auto ybin=1; ybin<=ny; ++ybin) {
            double yvalue = hist -> GetYaxis() -> GetBinCenter(ybin);
            if (graph -> IsInside(xvalue,yvalue)) {
                double value = f2 -> Eval(xvalue,yvalue);
                integral += value*binA;
            }
        }
    }
    return integral;
}

double Integral2DGaussian(TF2 *f2, double contoA=0) { return Integral2DGaussian(f2->GetParameter(0), f2->GetParameter(2), f2->GetParameter(4), contoA); }
double Integral2DGaussian(double amplitude, double sigma_x, double sigma_y, double contoA) { return amplitude*2*TMath::Pi()*sigma_x*sigma_y*(1-contoA/amplitude); }
void ResetBinning() { fBnn2.SetNMM(800,-1025,-875,800,0,200); fBnn3.SetNMM(800,-1025,-875,800,0,200); CreateAndFillHistogram(1); }
void SetBinningX(int nx, double x1, double x2, int fill) { fBnn2.SetXNMM(nx,x1,x2); fBnn3.SetXNMM(nx,x1,x2); if (fill) CreateAndFillHistogram(1); }
void SetBinningY(int ny, double y1, double y2, int fill) { fBnn2.SetYNMM(ny,y1,y2); fBnn3.SetYNMM(ny,y1,y2); if (fill) CreateAndFillHistogram(1); }
void SetBinningX(double x1, double x2, int fill) { fBnn2.SetXMM(x1,x2); fBnn3.SetXMM(x1,x2); if (fill) CreateAndFillHistogram(1); }
void SetBinningY(double y1, double y2, int fill) { fBnn2.SetYMM(y1,y2); fBnn3.SetYMM(y1,y2); if (fill) CreateAndFillHistogram(1); }
void SetBinning(int nx, double x1, double x2, int ny, double y1, double y2) { SetBinningX(nx,x1,x2,0); SetBinningY(ny,y1,y2,0); CreateAndFillHistogram(1); }
void SetBinning(double x1, double x2, double y1, double y2) { SetBinningX(x1,x2,0); SetBinningY(y1,y2,0); CreateAndFillHistogram(1); }
void SetBinWX(double w, int fill) { fBnn2.SetWX(w); fBnn3.SetWX(w); if (fill) CreateAndFillHistogram(1); }
void SetBinWY(double w, int fill) { fBnn2.SetWY(w); fBnn3.SetWY(w); if (fill) CreateAndFillHistogram(1); }
void SetBinNX(double n, int fill) { fBnn2.SetNX(n); fBnn3.SetNX(n); if (fill) CreateAndFillHistogram(1); }
void SetBinNY(double n, int fill) { fBnn2.SetNY(n); fBnn3.SetNY(n); if (fill) CreateAndFillHistogram(1); }
void SaveBinning() {
    if (!fHistPID) return;
    fBnn2.SetBinning(fHistPID,true);
    fBnn3.SetBinning(fHistPID,true);
    CreateAndFillHistogram(1);
}

void SetContourAScale(double scale) {
    if (scale<0) {
        e_note << "Enter fit range from 0.1 to 0.9 (1 default): ";
        TString inputString;
        cin >> inputString;
        inputString = inputString.Strip(TString::kBoth);
        scale = inputString.Atof();
    }
    fFinalContourAScale = scale;
    cout << "   " << fFinalContourAScale << endl;
}

void ChangeSigDist(double sigDist) {
    if (sigDist<0) {
        e_note << "Enter fit range from 0 to 5 (1 default): ";
        TString inputString;
        cin >> inputString;
        inputString = inputString.Strip(TString::kBoth);
        sigDist = inputString.Atof();
    }
    fSigDist = sigDist;
    cout << "   " << fSigDist << endl;
}

void Guide(TString mode)
{
    if (mode.Index("i")>=0) {
        e_note << "List of binning methods:" << endl;
          cout << "   SetBinningX(x1, x2)" << endl;
          cout << "   SetBinningY(y1, y2)" << endl;
          cout << "   SetBinningX(nx, x1, x2)" << endl;
          cout << "   SetBinningY(ny, y1, y2)" << endl;
          cout << "   SetBinning(x1, x2, y1, y2)" << endl;
          cout << "   SetBinning(nx, x1, x2, ny, y1, y2)" << endl;
          cout << "   SetBinWX(w)" << endl;
          cout << "   SetBinWY(w)" << endl;
          cout << "   SetBinNX(n)" << endl;
          cout << "   SetBinNY(n)" << endl;
          cout << "   SaveBinning()" << endl;
    }
    else                    e_info << "Enter '_help'         to print list of methods for binning setting" << endl;
                            e_info << "Enter '_reset_bin'    to reset binning configuration" << endl;
                            e_info << "Enter '_save_bin'     to save current binning configuration" << endl;
                            e_info << "Enter '_set_contour'  to change the final contour amplitude scale (" << fFinalContourAScale << ")" << endl;
    if (mode.Index("a")>=0) e_note << "Enter '_list_files'   to print list of files" << endl;
    if (mode.Index("s")>=0) e_note << "Enter '_select_files' to select files" << endl;
    if (mode.Index("d")>=0) e_note << "Enter '_draw_points'  to draw and select PID centers" << endl;
    if (mode.Index("t")>=0) e_note << "Enter '_draw_pid'     to redraw" << endl;
    if (mode.Index("r")>=0) e_note << "Enter '_select_pid'   to redraw and select PID centers" << endl;
    if (mode.Index("f")>=0) e_note << "Enter '_fit_total'    to fit histogram simultaneously using previous fits" << endl;
    if (mode.Index("g")>=0) e_note << "Enter '_summary'      to make summary of the result" << endl;
    if (mode.Index("q")>=0) e_info << "Enter '_set_sigdist'  to change fit range" << endl;
    //else                    e_info << "Enter 'ii' to print list of methods for binning setting" << endl;
    //                        e_info << "Enter 'bb' to reset binning configuration" << endl;
    //                        e_info << "Enter 'vv' to save current binning configuration" << endl;
    //                        e_info << "Enter 'ee' to change the final contour amplitude scale (" << fFinalContourAScale << ")" << endl;
    //if (mode.Index("a")>=0) e_note << "Enter 'aa' to print list of files" << endl;
    //if (mode.Index("s")>=0) e_note << "Enter 'ss' to select files" << endl;
    //if (mode.Index("d")>=0) e_note << "Enter 'dd' to draw and select PID centers" << endl;
    //if (mode.Index("t")>=0) e_note << "Enter 'tt' to redraw" << endl;
    //if (mode.Index("r")>=0) e_note << "Enter 'rr' to redraw and select PID centers" << endl;
    //if (mode.Index("f")>=0) e_note << "Enter 'ff' to fit histogram simultaneously using previous fits" << endl;
    //if (mode.Index("g")>=0) e_note << "Enter 'gg' to make summary of the result" << endl;
    //if (mode.Index("q")>=0) e_info << "Enter 'qq' to change fit range" << endl;
}

void MakeSummary()
{
    e_title << "MakeSummary" << endl;
    //TString summaryName1 = fCurrentFileName;
    //summaryName1.ReplaceAll("gen.root",".txt");
    //summaryName1.ReplaceAll("/chkf2run","/run");
    //if (summaryName1.Index("./")==0)
    //    summaryName1 = summaryName1(2,summaryName1.Sizeof());
    //summaryName1.ReplaceAll("/","_");
    //summaryName1 = TString("summary/summary_") + summaryName1;
    TString summaryName2 = Form("summary/run_%04d.txt",fCurrentRunNumber);;
    e_info << "Summary files " << endl;
    for (TString summaryName : {summaryName2}) cout << "   " << summaryName << endl;
    for (TString summaryName : {summaryName2})
    {
        TString yname;
        if (fCurrentType==2) yname = "y_f2ssde";
        if (fCurrentType==3) yname = "y_f3ssde";
        ofstream fileSummary(summaryName);
        auto numFits = fFitArray -> GetEntries();
        for (auto iFit=0; iFit<numFits; ++iFit)
        {
            auto total = fHistPID -> GetEntries();
            TH1D *histData = (TH1D*) fHistDataArray -> At(iFit);
            TH1D *histBack = (TH1D*) fHistBackArray -> At(iFit);
            auto bin = histData -> FindBin(fFinalContourAScale+0.5*(1./fNumContours));
            auto count = histData -> GetBinContent(bin);
            auto contamination = histBack -> GetBinContent(bin);
            auto corrected = count - contamination;
            auto fit = (TF2*) fFitArray -> At(iFit);
            auto amplit = fit->GetParameter(0);
            auto valueX = fit->GetParameter(1);
            auto sigmaX = fit->GetParameter(2);
            auto valueY = fit->GetParameter(3);
            auto sigmaY = fit->GetParameter(4);
            auto thetaR = fit->GetParameter(5);
            fileSummary << "####################################################" << endl;
            fileSummary << left;
            fileSummary << setw(25) << Form("pid%d/total",iFit)              << total << endl;
            fileSummary << setw(25) << Form("pid%d/ca_scale",iFit)           << fFinalContourAScale << endl;
            fileSummary << setw(25) << Form("pid%d/count",iFit)              << count << endl;
            fileSummary << setw(25) << Form("pid%d/contamination",iFit)      << contamination << endl;
            fileSummary << setw(25) << Form("pid%d/corrected",iFit)          << corrected << endl;
            fileSummary << setw(25) << Form("pid%d/purity",iFit)             << corrected/total << endl;
            fileSummary << setw(25) << Form("pid%d/amplitude",iFit)          << amplit << endl;
            fileSummary << setw(25) << Form("pid%d/x(x_rf0)",iFit)           << valueX << endl;
            fileSummary << setw(25) << Form("pid%d/y(%s)",iFit,yname.Data()) << valueY << endl;
            fileSummary << setw(25) << Form("pid%d/sigma_x",iFit)            << sigmaX << endl;
            fileSummary << setw(25) << Form("pid%d/sigma_y",iFit)            << sigmaY << endl;
            fileSummary << setw(25) << Form("pid%d/theta_deg",iFit)          << thetaR*TMath::RadToDeg() << endl;
            if (fFinalContourGraph!=nullptr) {
                auto numPoints = fFinalContourGraph -> GetN();
                fileSummary << setw(25) << Form("pid%d/contour_x  ",iFit);
                for (auto iPoint=0; iPoint<numPoints; ++iPoint) fileSummary << fFinalContourGraph -> GetPointX(iPoint) << ","; fileSummary << endl;
                fileSummary << setw(25) << Form("pid%d/contour_y  ",iFit);
                for (auto iPoint=0; iPoint<numPoints; ++iPoint) fileSummary << fFinalContourGraph -> GetPointY(iPoint) << ","; fileSummary << endl;
            }
            fileSummary << endl;
        }
    }

    auto cvsPID = fGroupPID -> GetCanvas();
    auto cvsFit = fGroupFit -> GetCanvas();
    if (cvsPID!=nullptr) cvsPID -> SaveAs(Form("figures/pid_%04d.png",fCurrentRunNumber));
    if (cvsFit!=nullptr) cvsFit -> SaveAs(Form("figures/fit_%04d.png",fCurrentRunNumber));

    Guide("ra");
}
