void test()
    {
   TCanvas *c1 = new TCanvas("c1","transparent pad",200,10,700,500);
   TPad *pad1 = new TPad("pad1","",0,0,1,1);
   TPad *pad2 = new TPad("pad2","",0,0,1,1);
   pad2->SetFillColor(0);
   pad2->SetFillStyle(4000);
   pad2->SetFrameFillStyle(0);

   pad1->Draw();
   pad1->cd();

   TH1F *h1 = new TH1F("h1","h1",100,-3,3);
   TRandom r;
   for (Int_t i=0;i<100000;i++) {
      Double_t x1 = r.Gaus(-1,0.5);
      h1->Fill(x1);
   }
   h1->Draw();
   
   pad2->Draw();
   pad2->cd();
   h1->Draw("Y+");
   c1->Print("./test.png");
}
