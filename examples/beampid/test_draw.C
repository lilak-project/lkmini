void test_draw()
{
    auto pid = new LKBeamPIDControl;
    pid -> ListFiles(".","gen.root");
    pid -> SelectFile(0);

    pid -> SelectCenters(
        vector<vector<double>>{
            vector<double>{-927.385807, 131.003489},
            vector<double>{-935.410841, 125.175398},
            vector<double>{-927.347709, 123.433259},
        }
    );
    pid -> CalibrationRun();

    pid -> SelectCenters(
        vector<vector<double>>{
            vector<double>{-927.385807, 131.003489},
            vector<double>{-935.410841, 125.175398},
            vector<double>{-927.347709, 123.433259},
        }
    );
    pid -> FitTotal();
    pid -> MakeSummary();
}
