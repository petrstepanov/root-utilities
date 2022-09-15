#include "CanvasHelper.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <TVirtualPad.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TH1.h>
#include <TF1.h>
#include <TGraph.h>
#include <TRandom3.h>

void demo() {

#ifdef __CINT__
    // Load CanvasHelper library
    gSystem->Load("CanvasHelper_cpp.so");
  #endif

  // Instantiate and divide canvas
  TCanvas *c = new TCanvas();
  c->SetWindowSize(850, 650);

  // Divide canvas and leave no spacing between pads (looks better)
  c->Divide(2, 2, 1E-5, 1E-5);

  // PAD 1: draw histogram and fit it
  TVirtualPad *pad1 = c->cd(1);
  TH1 *h = new TH1D("hist", "Histogram from a Gaussian;Coordinate, m;Events", 100, -5, 15);
  h->FillRandom("gaus", 10000);
  h->Draw();
  gStyle->SetOptFit(111);
  h->Fit("gaus");

  // FEATURE 1: obtain default stat box from the canvas and align it to the left top corner
  TPave *pave = CanvasHelper::getDefaultPaveStats(pad1);
  CanvasHelper::setPaveAlignment(pave, CanvasHelper::kPaveAlignRight | CanvasHelper::kPaveAlignTop);

  // PAD 2: draw a graph
  c->cd(2);
  TGraph *g = new TGraph(h);
  g->SetTitle("Graph from ROOT Manual; Array indexes; Array values");
  g->Draw();

  // PAD 3: draw a function
  c->cd(3);
  TF1 *fa1 = new TF1("fa1", "sin(x)/x", 0, 10);
  fa1->GetXaxis()->SetTitle("X coordinate");
  fa1->GetYaxis()->SetTitle("Function value");
  fa1->Draw();

  // PAD 4: draw a function
  TVirtualPad *pad4 = c->cd(4);
  gStyle->SetOptStat(0);

  auto h1 = new TH1F("h1", "Histogram 1;X axis;Y axis", 100, -4, 4);
  auto h2 = new TH1F("h2", "Histogram 2", 100, -4, 4);

  TRandom3 rng;
  Double_t px, py;
  for (Int_t i = 0; i < 25000; i++) {
    rng.Rannor(px, py);
    h1->Fill(px, 10.);
    h2->Fill(px, 8.);
  }

  h1->SetMarkerStyle(kFullCircle);
  h2->SetMarkerStyle(kFullSquare);

  h1->Draw("PLC PMC");
  h2->Draw("SAME PLC PMC");


  pad4->BuildLegend();
  h1->SetTitle("Two Histograms");
  // FEATURE 3: align a legend
  TLegend* legend = CanvasHelper::getDefaultLegend(pad4);
  CanvasHelper::setPaveAlignment(legend, CanvasHelper::kPaveAlignLeft | CanvasHelper::kPaveAlignTop);

  // FEATURE 4: add title and/or subtitle to a multi-pad canvas
  CanvasHelper::addMultiCanvasTitle(c, "Multi-Pad Canvas Title", "Canvas features a ROOT histogram and a graph.");

  // Pass canvas for processing
  CanvasHelper::getInstance()->addCanvas(c);

  c->Modified();
  c->Paint();
  c->Update();
}

#ifndef __CINT__

int main(int argc, char **argv) {
  // Instantiate TApplication
  TApplication app("demo", &argc, argv);

  // TODO: Call your ROOT script entry function
  demo();

  // Enter the event loop
  app.Run();

  // Return success
  return 0;
}

#endif
