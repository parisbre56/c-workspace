function createPlots (q1,q2,qs,q1bc,q2bc,qsbc,q1asbc,q2asbc,qsasbc,p,kap,textStr)
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
  
  %plotting time
  figure();
  for k=1:size(kap,1)
    subplot(size(kap,1),1,k);
    plot(p,q1(k,:),"k-",...
      p,q2(k,:),"r-", ...
      p,q1bc(k,:),"g-",... 
      p,q2bc(k,:),"b-",...
      p,q1asbc(k,:),"m-", ...
      p,q2asbc(k,:),"c-");
    title(strcat("Time plot for:",textStr,", k=",num2str(kap(k))));
  end
  
  %plotting speedup
  figure();
  for k=1:size(kap,1)
    subplot(size(kap,1),1,k);
    plot(p,spd1(k,:),"k-",...
      p,spd2(k,:),"r-", ...
      p,spd1bc(k,:),"g-",... 
      p,spd2bc(k,:),"b-",...
      p,spd1asbc(k,:),"m-", ...
      p,spd2asbc(k,:),"c-");
    title(strcat("Speedup plot for:",textStr,", k=",num2str(kap(k))));
  end
  
  %plotting efficiency
  figure();
  for k=1:size(kap,1)
    subplot(size(kap,1),1,k);
    plot(p,eff1(k,:),"k-",...
      p,eff2(k,:),"r-", ...
      p,eff1bc(k,:),"g-",... 
      p,eff2bc(k,:),"b-",...
      p,eff1asbc(k,:),"m-", ...
      p,eff2asbc(k,:),"c-");
    title(strcat("Efficiency plot for:",textStr,", k=",num2str(kap(k))));
  end
  
  %black is continuous with send
  %red is shuffle with send
  %green is continuous with broadcast
  %blue is shuffle with broadcast
  %magenta is continuous with asynchronous broadcast
  %cyan is shuffle with asyncronous broadcast
  
end

function spdUp = computeSpeedup(multiProc,oneProc) 
  spdUp = oneProc./multiProc;
end

function eff = computeEfficiency(spdUp,procs)
  eff = spdUp./procs;
end