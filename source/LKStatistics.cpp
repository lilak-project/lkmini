#include "LKLogger.h"
#include "LKStatistics.h"

ClassImp(LKStatistics);

LKStatistics::LKStatistics()
{
    Clear();
}

LKStatistics::~LKStatistics()
{
}

void LKStatistics::Clear()
{
    fN = 0;
    fW = 0;
    fEX = 0;
    fEY = 0;
    fEZ = 0;
    fEXX = 0;
    fEYY = 0;
    fEZZ = 0;
    fEXY = 0;
    fEYZ = 0;
    fEZX = 0;
}

void LKStatistics::Add(Double_t x, Double_t w)
{
    Add(x,0,0,w);
}

void LKStatistics::Add(TVector3 pos, Double_t w)
{
    Add(pos.X(),pos.Y(),pos.Z(),w);
}

void LKStatistics::Add(Double_t x, Double_t y, Double_t z, Double_t w)
{
    Double_t wsum = fW + w;

    fEX  = (fW*fEX + w*x) / wsum;
    fEY  = (fW*fEY + w*y) / wsum;
    fEZ  = (fW*fEZ + w*z) / wsum;
    fEXX = (fW*fEXX + w*x*x) / wsum;
    fEYY = (fW*fEYY + w*y*y) / wsum;
    fEZZ = (fW*fEZZ + w*z*z) / wsum;
    fEXY = (fW*fEXY + w*x*y) / wsum;
    fEYZ = (fW*fEYZ + w*y*z) / wsum;
    fEZX = (fW*fEZX + w*z*x) / wsum;

    fW = wsum;
    ++fN;
}

bool LKStatistics::Subtract(TVector3 pos, Double_t w)
{
    return Subtract(pos.X(),pos.Y(),pos.Z(),w);
}

bool LKStatistics::Subtract(Double_t x, Double_t y, Double_t z, Double_t w)
{
    Double_t wsum = fW - w;

    fEX  = (fW * fEX - w * x) / wsum;
    fEY  = (fW * fEY - w * y) / wsum;
    fEZ  = (fW * fEZ - w * z) / wsum;
    fEXX = (fW * fEXX - w * x * x) / wsum;
    fEYY = (fW * fEYY - w * y * y) / wsum;
    fEZZ = (fW * fEZZ - w * z * z) / wsum;
    fEXY = (fW * fEXY - w * x * y) / wsum;
    fEYZ = (fW * fEYZ - w * y * z) / wsum;
    fEZX = (fW * fEZX - w * z * x) / wsum;

    fW = wsum;
    --fN;

    return true;
}

Int_t    LKStatistics::GetNumHits() const { return fN; };

Double_t LKStatistics::GetW()  const { return fW; }
Double_t LKStatistics::GetChargeSum()  const { return fW; }

Double_t LKStatistics::GetMeanX() const { return fEX; }
Double_t LKStatistics::GetMeanY() const { return fEY; }
Double_t LKStatistics::GetMeanZ() const { return fEZ; }

Double_t LKStatistics::GetMeanXX() const { return fEXX; }
Double_t LKStatistics::GetMeanYY() const { return fEYY; }
Double_t LKStatistics::GetMeanZZ() const { return fEZZ; }
Double_t LKStatistics::GetMeanXY() const { return fEXY; }
Double_t LKStatistics::GetMeanYZ() const { return fEYZ; }
Double_t LKStatistics::GetMeanZX() const { return fEZX; }

Double_t LKStatistics::GetVarianceX()  const { return fEXX - fEX * fEX; }
Double_t LKStatistics::GetVarianceY()  const { return fEYY - fEY * fEY; }
Double_t LKStatistics::GetVarianceZ()  const { return fEZZ - fEZ * fEZ; }

Double_t LKStatistics::GetAXX() const { return fW * (fEXX - fEX * fEX); }
Double_t LKStatistics::GetAYY() const { return fW * (fEYY - fEY * fEY); }
Double_t LKStatistics::GetAZZ() const { return fW * (fEZZ - fEZ * fEZ); }
Double_t LKStatistics::GetAXY() const { return fW * (fEXY - fEX * fEY); }
Double_t LKStatistics::GetAYZ() const { return fW * (fEYZ - fEY * fEZ); }
Double_t LKStatistics::GetAZX() const { return fW * (fEZX - fEZ * fEX); }

TVector3 LKStatistics::GetMean()          const { return TVector3(fEX,fEY,fEZ); }
TVector3 LKStatistics::GetVariance()      const { return TVector3(fEXX-fEX*fEX,fEYY-fEY*fEY,fEZZ-fEZ*fEZ); }
TVector3 LKStatistics::GetCovariance()    const { return TVector3(fEXY-fEX*fEY,fEYZ-fEY*fEZ,fEZX-fEZ*fEX); }
TVector3 LKStatistics::GetStdDev()        const { return TVector3(TMath::Sqrt(fEXX-fEX*fEX),TMath::Sqrt(fEYY-fEY*fEY),TMath::Sqrt(fEZZ-fEZ*fEZ)); }
TVector3 LKStatistics::GetSigma()         const { return GetStdDev(); }
TVector3 LKStatistics::GetSquaredMean()   const { return TVector3(fEXX,fEYY,fEZZ); }
TVector3 LKStatistics::GetCoSquaredMean() const { return TVector3(fEXY,fEYZ,fEZX); }

Double_t LKStatistics::GetXMean()          const { return GetMean()         .X(); }
Double_t LKStatistics::GetXVariance()      const { return GetVariance()     .X(); }
Double_t LKStatistics::GetXCovariance()    const { return GetCovariance()   .X(); }
Double_t LKStatistics::GetXStdDev()        const { return GetStdDev()       .X(); }
Double_t LKStatistics::GetXSigma()         const { return GetStdDev()       .X(); }
Double_t LKStatistics::GetXSquaredMean()   const { return GetSquaredMean()  .X(); }
Double_t LKStatistics::GetXCoSquaredMean() const { return GetCoSquaredMean().X(); }
