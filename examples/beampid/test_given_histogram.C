void test_given_histogram()
{
    auto file = new TFile("example_pid.root");
    TCanvas *cvs = LKPainter::GetPainter() -> CanvasResize("cvs",1,1,1);
    file -> Get("hist_pid") -> Draw();
    auto pid = new LKBeamPIDControl;
    pid -> SetRunNumber(83);
    pid -> UseCurrentgPad();

    pid -> SelectCenters(
    vector<vector<double>>{
        vector<double>{-919.906994, 144.416437},

        vector<double>{-927.375147, 130.953475},
        vector<double>{-935.388077, 125.137988},
        vector<double>{-927.362903, 123.482270},
        }
    );
    pid -> CalibrationRun();

    pid -> SelectCenters(
    vector<vector<double>>{
        vector<double>{-919.906994, 144.416437},

        vector<double>{-928.036790, 138.410959},
        vector<double>{-927.375147, 130.953475},
        vector<double>{-935.388077, 125.137988},
        vector<double>{-927.362903, 123.482270},
        vector<double>{-918.860941, 121.196785},
        vector<double>{-927.091839, 115.496863},
        vector<double>{-937.024217, 118.285243},
        vector<double>{-945.145963, 117.790890},

        //vector<double>{-936.420049, 110.043430},
        //vector<double>{-944.373391, 110.361144},
        //vector<double>{-936.150917, 101.292116},
        //vector<double>{-945.678947, 102.639950},
        //vector<double>{-953.129665, 104.939739},
        }
    );
    pid -> FitTotal();
    pid -> MakeSummary();
}
