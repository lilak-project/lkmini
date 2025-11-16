void draw_beam_pid()
{
    // example
    {
        auto file = new TFile("example_chkf2run0083gen.root");
        file -> Get("hist_pid") -> Draw("colz");
        return;
        LKBeamPID pid;
        pid.SetRunNumber(83);
        pid.UseCurrentgPad();
        vector<vector<double>> vv = {
            vector<double>{-927.581, 130.787},
            vector<double>{-934.872649, 125.220195},
            vector<double>{-927.581, 124}};
        pid.SelectCenters(vv);
        pid.FitTotal();
        pid.MakeSummary();
    }
    //{
    //    auto pid = new LKBeamPIDControl;
    //}
}
