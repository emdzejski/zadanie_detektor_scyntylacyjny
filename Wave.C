#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

Bool_t Wave(string n_file = "wave_0.dat", Int_t bl = 1){ //jesli argument bl == 1 przesuwamy
    //sygnal do zera
    fstream scyn;

    scyn.open(n_file, ios::binary | ios::in); //otwieranie pliku 
    
    if (scyn.is_open()){ 
        cout << "plik poprawnie załadowany" << endl;
    }
    else{
        cout << "błąd w ładowaniu pliku" << endl;
    }
    
    

    float* x;
    char * temp = new char[sizeof(float)]; //zmienne do konwersji pliku binarnego

    Int_t const sig_time = 1024;

    TCanvas *can = new TCanvas("can", "can", 700 ,400);
    TH1F *hSig = new TH1F("hSig","signal", sig_time, 0, sig_time); //tworzenie płótna i histogramu
    
    hSig->GetXaxis()->SetRangeUser(0,sig_time);
    hSig->GetYaxis()->SetRangeUser(400,1000);
    hSig->GetXaxis()->SetTitle("czas [ns]"); // ustalenie tytułów osi 
    hSig->GetYaxis()->SetTitle("amplituda [mV]");

    Int_t i = 0;
    Int_t j = 0; //zmienna pomocnicza do tworzenia tytułów histogramów

    vector<float> sig; //wektor przechowujący wartości jednego sygnału
        
    
    if(bl == 1){ //odejmowanie linii bazowej
        while(!scyn.eof()){
            scyn.read(temp, sizeof(float));
            x = (float*)temp;
            *x *= 1/4.096; // konwersja na mV
            
            
            i += 1;
            j += 1; 
            sig.push_back(*x);

            if(i%1024 == 0){
            //srednia z pierwszych 50 punktów
            Float_t sum = accumulate(sig.begin(), sig.begin() + 50, 0.0);
            Float_t avrg = sum/50;
            cout << Form("histogram %d przesunięty o: ", j/sig_time ) << avrg <<" mV"<<endl;   
                for(Int_t k=1; k<=sig_time; k++){
                    hSig->SetBinContent(k,sig[k-1]-avrg);
                    
                }
                
                can->cd();
                hSig->Draw();
                hSig->GetYaxis()->SetRangeUser(400-avrg,1000-avrg);
                hSig->SetTitle(Form("sygnal %i",j/sig_time));
                can->Update();
                can->WaitPrimitive();
                hSig->Reset();
                
                i=0;
                sig.clear();
                
            }
        }
    }

    else{ //bez odejmowania linii bazowej
        while(!scyn.eof()){ 
            scyn.read(temp, sizeof(float));
            x = (float*)temp;
            *x *= 1/4.096; // konwersja na mV
            
            
            i += 1;
            j += 1;
            sig.push_back(*x);
            hSig->SetBinContent(i,*x);

            if(i%1024 == 0){
            
                can->cd();
                hSig->Draw();
                hSig->SetTitle(Form("sygnal %i",j/sig_time));
                can->Update();
                can->WaitPrimitive();
                hSig->Reset();
                
                i=0;
            }
        }        
    }

    scyn.close();

    
    
    return kTRUE; 
}