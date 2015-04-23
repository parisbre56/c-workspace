procs=[2 4 6 8 10 12 14 16];
kappas=[4
16
32
64
];

%I
t1=[[.00009155273437499999 .00027096271514892578 .00049141645431518554 .00153700113296508788 .00343805551528930664 .00601502656936645507 .00931551456451416015 .01255819797515869140 ]
[.00044157505035400390 .00244466066360473632 .00566109418869018554 .02231667041778564453 .04127283096313476562 .08387542963027954101 .12619485855102539062 .18150999546051025390 ]
[.00179617404937744140 .01109474897384643554 .02504873275756835937 .10432041883468627929 .20530821084976196289 .34886837005615234374 .50990434885025024414 .76606742143630981445 ]
[.00969009399414062500 .05562746524810791015 .12492848634719848632 .50986239910125732421 .83329279422760009765 1.32383948564529418945 2.38790302276611328124 4.23688721656799316406 ]
];

%II
t2=[[.00010366439819335937 .00035673379898071289 .00061377286911010742 .00205360651016235351 .00440765619277954101 .00776190757751464843 .01239224672317504882 .01832641363143920898 ]
[.00048838853836059570 .00298442840576171874 .00686750411987304687 .02385193109512329101 .05166471004486083984 .08676916360855102539 .14662915468215942382 .22705214023590087890 ]
[.00185518264770507812 .01193926334381103515 .02678370475769042968 .09819988012313842773 .20507293939590454101 .34630221128463745117 .57102340459823608398 .90022031068801879882 ]
[.00960254669189453124 .05472068786621093750 .12131519317626953125 .45479512214660644531 .86943361759185791015 1.41974633932113647461 2.53713901042938232421 4.26359587907791137695 ]
];

%I Single proc
ts=[[.00000106096267700195 ]
[.00000462532043457031 ]
[.00002349615097045898 ]
[.00016614198684692382 ]
];

%Extra I
te1=[[.00001021623611450195 .00019040107727050781 .00039018392562866211 .00161970853805541992 .00364964008331298828 .00544539690017700195 .00842750072479248046 .01307150125503540039 ]
[.00051196813583374023 .00320149660110473632 .00727428197860717773 .02965474128723144531 .05796548128128051757 .10033255815505981445 .14784595966339111328 .22456904649734497070 ]
[.00316215753555297851 .01769090890884399414 .03983061313629150390 .16883975267410278320 .27714118957519531249 .49661684036254882812 .74983510971069335937 1.27652834653854370117 ]
[.02446401119232177734 .11532198190689086914 .27645760774612426757 1.01137132644653320312 1.78724262714385986328 3.13872232437133789062 6.16007969379425048828 9.07342693805694580078 ]
];

%Extra II
te2=[[.00001090764999389648 .00018758773803710937 .00039702653884887695 .00199824571609497070 .00372208356857299804 .00593992471694946289 .00985308885574340820 .01281248331069946289 ]
[.00050824880599975585 .00315793752670288086 .00738613605499267578 .03007266521453857421 .05616225004196166992 .09990115165710449218 .15476261377334594726 .22723673582077026367 ]
[.00314314365386962890 .01771473884582519531 .04123756885528564453 .16537934541702270507 .27629030942916870117 .48785102367401123046 .77288287878036499023 1.19716700315475463867 ]
[.02446488142013549804 .11425725221633911132 .27189756631851196289 1.05668567419052124023 1.80371747016906738281 3.18816882371902465820 6.07787744998931884765 9.20518516302108764648 ]
];

%Extra I Single proc
tes=[[.00000112056732177734 ]
[.00001270771026611328 ]
[.00008915662765502929 ]
[.00122387409210205077 ]
];

%BTest I
bt1=[[.00011917352676391601 .00032289028167724609 .00078041553497314453 .00172494649887084961 .00311862230300903320 .00671433210372924804 .01176117658615112304 .01753443479537963867 ]
[.00048699378967285156 .00311988592147827148 .00907672643661499023 .02568765878677368164 .05177383422851562500 .10275572538375854492 .16184028387069702148 .23103709220886230468 ]
[.00182926654815673828 .01372812986373901367 .03813831806182861328 .12397989034652709960 .27206138372421264648 .42992373704910278320 .69052259922027587890 .98783553838729858398 ]
[.00993736982345581054 .06544475555419921874 .17962770462036132812 .59627283811569213867 1.19526954889297485351 1.71327871084213256835 3.12662149667739868164 5.05407531261444091796 ]
];

%BTest II
bt2=[[.00007799863815307617 .00044507980346679687 .00085902214050292968 .00193004608154296875 .00356010198593139648 .00739803314208984374 .01647834777832031250 .01512827873229980468 ]
[.00055727958679199218 .00333615541458129882 .00940749645233154296 .02326370477676391601 .04308595657348632812 .08573148250579833984 .13715205192565917968 .21968352794647216796 ]
[.00206941366195678711 .01382960081100463867 .03810892105102539062 .10275036096572875976 .20473483800888061523 .37732647657394409179 .63290245532989501953 .92554551362991333007 ]
[.01015667915344238281 .06280237436294555664 .17035274505615234375 .48903970718383789062 .92835870981216430663 1.53666183948516845703 2.81573218107223510742 4.57349678277969360351 ]
];

%BTest I Single proc
bts=[[.00000100135803222656 ]
[.00000480413436889648 ]
[.00002380609512329101 ]
[.00016623735427856445 ]
];

%BTest Extra I
bte1=[[.00002840757369995116 .00021075010299682617 .00061606168746948242 .00158644914627075195 .00546380281448364257 .00653463602066040039 .01169263124465942382 .01505782604217529296 ]
[.00052387714385986328 .00369911193847656250 .01010876893997192382 .03142327070236206054 .05919591188430786132 .11680910587310791015 .18567488193511962890 .30308711528778076171 ]
[.00314106941223144531 .02093579769134521484 .04859963655471801757 .17914005517959594726 .32798027992248535156 .60536724328994750976 .90604826211929321288 1.38781890869140625000 ]
[.02459564208984374999 .12965838909149169921 .39627431631088256835 1.06998739242553710937 1.90614681243896484374 3.31504667997360229492 6.46833437681198120117 9.56830843687057495117 ]
];

%BTest Extra II
bte2=[[.00002897977828979492 .00021009445190429687 .00061664581298828125 .00165104866027832031 .00365563631057739257 .00688527822494506835 .01111730337142944336 .01609008312225341796 ]
[.00051380395889282226 .00370322465896606445 .01018539667129516601 .03091661930084228515 .06332002878189086914 .11547272205352783203 .19164506196975708008 .29113467931747436523 ]
[.00318914651870727539 .02115651369094848632 .04873516559600830078 .17252744436264038085 .31617479324340820312 .58731029033660888671 .89615873098373413085 1.38199046850204467773 ]
[.02452057600021362304 .12943724393844604492 .30028342008590698242 1.08913773298263549804 1.96350568532943725586 3.34609897136688232421 6.50696243047714233398 9.84075404405593872070 ]
];

%BTest Extra I Single proc
btes=[[.00000158548355102539 ]
[.00001311302185058594 ]
[.00009460449218750000 ]
[.00126208066940307617 ]
];

%AsBTest I
abt1=[[.00017596483230590820 .00061706304550170898 .00170527696609497070 .01544505357742309570 .02850832939147949218 .05375746488571166992 .08391059637069702148 .15538202524185180664 ]
[.00168287754058837890 .00757870674133300781 .02631525993347167968 .24055484533309936523 .42294481992721557617 .74536209106445312500 1.28267204761505126953 1.75026558637619018554 ]
[.00700812339782714843 .03023332357406616210 .10819214582443237304 .97053037881851196288 1.67267479896545410156 2.57823596000671386718 4.75258414745330810546 6.83467499017715454101 ]
[.03026956319808959961 .13893754482269287109 .44114843606948852539 3.11746848821640014648 6.67191418409347534179 9.86738260984420776367 18.44626982212066650390 26.59535455703735351562 ]
];

%AsBTest II
abt2=[[.00014339685440063476 .00064109563827514648 .00180426836013793945 .01533925533294677734 .02952371835708618163 .05669971704483032226 .09477977752685546875 .12468950748443603515 ]
[.00158610343933105468 .00765496492385864257 .02611844539642333984 .24171187877655029296 .50090918540954589843 .77817500829696655273 1.36590108871459960937 1.84947417974472045898 ]
[.00644751787185668945 .03060437440872192382 .11275337934494018554 .99308778047561645507 1.68976843357086181640 2.53538048267364501953 5.57918789386749267578 7.53585512638092041015 ]
[.02814704179763793945 .13052322864532470703 .57872631549835205078 4.18073234558105468750 7.69741582870483398437 10.70485411882400512695 22.17769093513488769531 30.47490692138671875000 ]
];

%AsBTest I Single proc
abts=[[.00000101327896118164 ]
[.00000491142272949218 ]
[.00002381801605224609 ]
[.00016735792160034179 ]
];

%AsBTest Extra I
abte1=[[.00008745193481445312 .00050331354141235351 .00158139467239379882 .01342395544052124023 .02722368240356445312 .04397602081298828125 .08949533700942993164 .11291737556457519531 ]
[.00130916833877563476 .00713251829147338867 .02597882747650146484 .22928725481033325195 .44580579996109008789 .71863797903060913085 1.28022049665451049804 1.69177694320678710937 ]
[.00557228326797485351 .03151166439056396484 .10715740919113159179 .80887676477432250976 1.71471521854400634765 2.46406832933425903320 4.69212994575500488281 6.68006845712661743164 ]
[.02802258729934692382 .14318999052047729492 .49459630250930786132 2.79976177215576171875 6.71039386987686157226 11.85237165689468383789 19.99860035181045532226 29.79418075084686279296 ]
];

%AsBTest Extra II
abte2=[[.00009276866912841796 .00050184726715087890 .00157468318939208984 .01423031091690063476 .02793697118759155273 .05244238376617431640 .07763732671737670898 .14174153804779052734 ]
[.00127515792846679687 .00746103525161743164 .02586237192153930664 .21996669769287109375 .42421469688415527343 .69835929870605468750 1.23090602159500122070 1.70169582366943359374 ]
[.00562176704406738281 .03067916631698608398 .10913044214248657226 .81043077707290649414 1.68247295618057250976 2.58058134317398071289 4.58165113925933837890 6.82050154209136962890 ]
[.02707372903823852538 .14854580163955688476 .50410478115081787109 2.79350186586380004882 6.51437813043594360351 12.08067848682403564453 20.50524286031723022460 30.16652781963348388672 ]
];

%AsBTest Extra I Single proc
abtes=[[.00000101327896118164 ]
[.00001229047775268554 ]
[.00008965730667114257 ]
[.00122396945953369140 ]
];
