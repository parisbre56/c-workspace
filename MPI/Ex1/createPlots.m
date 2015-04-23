function createPlots (q1,q2,qs,q1bc,q2bc,qsbc,q1asbc,q2asbc,qsasbc,p,k)
  spd1=computeSpeedup(q1,qs);
  eff1=computeEfficiency(spd1,p);
  
  spd2=computeSpeedup(q2,qs);
  eff2=computeEfficiency(spd2,p);
  
  spd1bc=computeSpeedup(q1bc,qsbc);
  eff1bc=computeEfficiency(spd1bc,p);
  
  spd2bc=computeSpeedup(q2bc,qsbc);
  eff2bc=computeEfficiency(spd2bc,p);
  
  spd1asbc=computeSpeedup(q1asbc,qsasbc);
  eff1asbc=computeEfficiency(spd1asbc,p);
  
  spd2asbc=computeSpeedup(q2asbc,qsasbc);
  eff2asbc=computeEfficiency(spd2asbc,p);
  
  %ftiaxe ena figure gia xrono, ena gia speedup kai ena gia efficiency
  %gia kathe k, kane allo subplot
  
  %computing time
  
end

function spdUp = computeSpeedup(multiProc,oneProc) 
  spdUp = oneProc./multiProc;
end

function eff = computeEfficiency(spdUp,procs)
  eff = spdUp./procs;
end