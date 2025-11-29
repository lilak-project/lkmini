#ifndef LKSTATISTICS_HH
#define LKSTATISTICS_HH

#include "TVector3.h"

class LKStatistics
{
    private:
        Int_t fN = 0;  ///< Number of hits
        Double_t fW = 0;  ///< Sum of charge
        Double_t fEX  = 0;  ///< \<x>   Mean value of x
        Double_t fEY  = 0;  ///< \<y>   Mean value of y
        Double_t fEZ  = 0;  ///< \<z>   Mean value of z
        Double_t fEXX = 0;  ///< \<x*x> Mean value of x*x
        Double_t fEYY = 0;  ///< \<y*y> Mean value of y*y
        Double_t fEZZ = 0;  ///< \<z*z> Mean value of z*z
        Double_t fEXY = 0;  ///< \<x*y> Mean value of x*y
        Double_t fEYZ = 0;  ///< \<y*z> Mean value of y*z
        Double_t fEZX = 0;  ///< \<z*x> Mean value of z*y

    public:
        LKStatistics();
        virtual ~LKStatistics();

        void Clear();
        void Add(Double_t x, Double_t w=1);
        void Add(TVector3 pos, Double_t w=1);
        void Add(Double_t x, Double_t y, Double_t z, Double_t w=1);
        bool Subtract(TVector3 pos, Double_t w=1);
        bool Subtract(Double_t x, Double_t y, Double_t z, Double_t w=1);

        Int_t GetNumHits() const;
        Double_t GetW() const;
        Double_t GetChargeSum() const;

        Double_t GetMeanX() const;
        Double_t GetMeanY() const;
        Double_t GetMeanZ() const;

        Double_t GetMeanXX() const;
        Double_t GetMeanYY() const;
        Double_t GetMeanZZ() const;
        Double_t GetMeanXY() const;
        Double_t GetMeanYZ() const;
        Double_t GetMeanZX() const;

        Double_t GetVarianceX() const; ///< SUM_i (\<x>-x_i)^2
        Double_t GetVarianceY() const; ///< SUM_i (\<y>-y_i)^2
        Double_t GetVarianceZ() const; ///< SUM_i (\<z>-z_i)^2

        Double_t GetAXX() const; ///< W * SUM_i (\<x>-x_i)^2 : diagonal compoent of matrix A
        Double_t GetAYY() const; ///< W * SUM_i (\<y>-y_i)^2 : diagonal compoent of matrix A
        Double_t GetAZZ() const; ///< W * SUM_i (\<z>-z_i)^2 : diagonal compoent of matrix A
        Double_t GetAXY() const; ///< W * SUM_i (\<x>-x_i)*(\<y>-y_i) : off-diagonal compoent of matrix A
        Double_t GetAYZ() const; ///< W * SUM_i (\<y>-y_i)*(\<z>-z_i) : off-diagonal compoent of matrix A
        Double_t GetAZX() const; ///< W * SUM_i (\<z>-z_i)*(\<x>-x_i) : off-diagonal compoent of matrix A

        TVector3 GetMean()          const;
        TVector3 GetVariance()      const;
        TVector3 GetCovariance()    const;
        TVector3 GetStdDev()        const;
        TVector3 GetSigma()         const;
        TVector3 GetSquaredMean()   const;
        TVector3 GetCoSquaredMean() const;

        Double_t GetXMean()          const;
        Double_t GetXVariance()      const;
        Double_t GetXCovariance()    const;
        Double_t GetXStdDev()        const;
        Double_t GetXSigma()         const;
        Double_t GetXSquaredMean()   const;
        Double_t GetXCoSquaredMean() const;

    ClassDef(LKStatistics, 1);
};

#endif
