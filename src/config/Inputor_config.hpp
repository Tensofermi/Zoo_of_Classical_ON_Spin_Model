#pragma once

void Inputor::initInputor()
{
    setGroup("Model_Parameters");
    addInputor(para.Nspin         ,   "N"               , 		    1                 );
    addInputor(para.D             ,   "D"               , 		    2                 );
    addInputor(para.beta          ,   "beta"            , 		    0.4               );
    addInputor(para.h             ,   "h"               , 		    0                 );
    addInputor(para.L             ,   "L"               , 		    8                 );
    
}

void Inputor::validateInputor()
{
    std::ostringstream errors;
    if (para.Nspin <= 0) errors << "N must be positive\n";
    if (para.D <= 0) errors << "D must be positive\n";
    if (para.L <= 0) errors << "L must be positive\n";
    if (!(para.beta >= 0.0) || !std::isfinite(para.beta)) errors << "beta must be finite and non-negative\n";
    if (para.N_Measure == 0) errors << "N_Measure must be positive\n";
    if (para.N_Each == 0) errors << "N_Each must be positive\n";
    if (para.N_Total == 0) errors << "N_Total must be positive\n";
    if (para.NBlock < 2) errors << "NBlock must be at least 2\n";
    if (para.MaxNBin < para.NBlock) errors << "MaxNBin must be at least NBlock\n";
    if (para.NperBin == 0) errors << "NperBin must be positive\n";
    if (!errors.str().empty()) io.exportInfo(io.ErrorInfo, errors.str());
}
