procs=[2 4 6 8 10 12 14 16];
kappas=[4
16
32
64
];

%I
t1=[[.00009380578994750976 .00026736259460449218 .00050376653671264648 .00170078277587890624 .00354738235473632812 .00702896118164062499 .00980516672134399414 .01406406164169311523 ]
[.00050990581512451171 .00270457267761230468 .00643880367279052734 .02589569091796875000 .05177125930786132812 .08603599071502685546 .13683544397354125976 .19844948053359985351 ]
[.00227994918823242187 .01273728609085083007 .03000211715698242187 .12133382558822631836 .20645114183425903320 .37455151081085205078 .61262609958648681640 .89147740602493286132 ]
[.01278069019317626953 .07186464071273803710 .16033064126968383789 .68818398714065551757 1.08722093105316162109 1.72722935676574707031 3.56042567491531372070 5.73203904628753662109 ]
];

%II
t2=[[.00011309385299682617 .00037220716476440429 .00062500238418579101 .00210157632827758789 .00453506708145141601 .00799242258071899414 .01366419792175292968 .01954549551010131835 ]
[.00056053400039672851 .00324913263320922851 .00752075910568237304 .02633531093597412109 .05706173181533813476 .10114589929580688476 .16370455026626586914 .24605692625045776367 ]
[.00235394239425659179 .01391180753707885742 .03207652568817138671 .11754746437072753906 .24342559576034545898 .39836558103561401367 .65503301620483398437 .97450685501098632812 ]
[.01224398612976074218 .07219966650009155273 .16148637533187866210 .60176918506622314453 1.13726025819778442382 2.05014448165893554687 3.86283404827117919921 5.91465898752212524414 ]
];

%I Single proc
ts=[[.00000172853469848632 ]
[.00001878738403320312 ]
[.00012701749801635742 ]
[.00097284317016601562 ]
];

%Extra I
te1=[[.00001251697540283203 .00020281076431274414 .00042706727981567382 .00170903205871582031 .00360159873962402343 .00632292032241821289 .00927375555038452148 .01454625129699707031 ]
[.00075788497924804687 .00390275716781616210 .00949232578277587890 .03617601394653320312 .07580261230468750000 .13301663398742675781 .20205688476562499999 .29744510650634765625 ]
[.00537335872650146484 .02380205392837524414 .05779242515563964843 .23065972328186035156 .44897996187210083007 .66522898674011230468 1.13008687496185302734 1.71346032619476318359 ]
[.04373309612274169921 .17598849534988403320 .39775437116622924804 1.63637620210647583007 3.38269846439361572265 5.52924920320510864257 9.88423843383789062500 14.49841837882995605468 ]
];

%Extra II
te2=[[.00001565217971801757 .00019619464874267578 .00042312145233154296 .00164428949356079101 .00366787910461425781 .00666610002517700195 .01058646440505981445 .01508862972259521484 ]
[.00075629949569702148 .00389181375503540039 .00949845314025878906 .03765304088592529296 .07707616090774536132 .12660624980926513671 .20046496391296386718 .28986177444458007812 ]
[.00534279346466064453 .02396599054336547851 .05790861845016479492 .23132539987564086914 .41851584911346435547 .64970667362213134765 1.12934410572052001953 1.83246277570724487304 ]
[.04207619428634643554 .17391469478607177734 .38929373025894165039 1.68152279853820800781 3.34094547033309936523 5.56419570446014404296 9.72354739904403686523 14.81428041458129882812 ]
];

%Extra I Single proc
tes=[[.00000230073928833007 ]
[.00006284713745117187 ]
[.00049488544464111328 ]
[.00451525449752807617 ]
];

%BTest I
bt1=[[.00013474225997924804 .00032989978790283203 .00080069303512573242 .00178955793380737304 .00324996709823608398 .00677428245544433593 .01083500385284423828 .01865715980529785156 ]
[.00055075883865356445 .00340147018432617187 .00967187881469726562 .02803573608398437500 .05875328779220581054 .11197907924652099609 .17407379150390625000 .24843813180923461914 ]
[.00235233306884765625 .01536566019058227539 .04284017086029052734 .13635845184326171875 .28654946088790893554 .48273425102233886718 .76847953796386718750 1.13565940856933593750 ]
[.01278176307678222656 .08390804529190063476 .21372804641723632812 .73744430541992187500 1.31408042907714843750 2.18417325019836425781 4.36195064783096313476 6.77013041973114013671 ]
];

%BTest II
bt2=[[.00009028911590576172 .00043507814407348632 .00086559057235717773 .00206059217453002929 .00405735969543457031 .00657215118408203125 .01146401166915893554 .01607795953750610351 ]
[.00063706636428833007 .00360090732574462890 .00991684198379516601 .02469774484634399414 .05008846521377563476 .09982481002807617187 .15398963689804077148 .24603706598281860351 ]
[.00249154567718505859 .01522890329360961913 .04188292026519775390 .11473226547241210937 .28275141716003417968 .36612433195114135742 .70347845554351806640 1.02938517332077026367 ]
[.01246569156646728515 .07575930356979370117 .20161870718002319335 .59978456497192382812 1.02537879943847656249 1.75364884138107299804 4.00525822639465332031 6.09971903562545776367 ]
];

%BTest I Single proc
bts=[[.00000131130218505859 ]
[.00001882314682006836 ]
[.00012891292572021484 ]
[.00096929073333740234 ]
];

%BTest Extra I
bte1=[[.00003054141998291015 .00022659301757812499 .00065279006958007812 .00186777114868164062 .00455890893936157226 .00796078443527221679 .01157774925231933593 .01655956506729125976 ]
[.00074263811111450195 .00453151464462280273 .01199342012405395507 .03927434682846069336 .08901498317718505859 .15934789180755615234 .24072132110595703125 .32773537635803222656 ]
[.00517303943634033203 .02776991128921508789 .06757867336273193359 .24127626419067382812 .41929506063461303710 .70303200483322143554 1.23112288713455200195 1.78592939376831054687 ]
[.04401170015335083008 .19633957147598266601 .44371001720428466796 1.65617562532424926757 3.51831899881362915039 5.26199094057083129882 10.21091120243072509765 14.73894660472869873046 ]
];

%BTest Extra II
bte2=[[.00003669261932373046 .00022500753402709961 .00064859390258789062 .00179305076599121093 .00443223714828491211 .00733150243759155273 .01198844909667968750 .01797091960906982421 ]
[.00073096752166748046 .00455347299575805664 .01192836761474609375 .03839514255523681640 .08060970306396484374 .13664132356643676757 .23063243627548217773 .33246065378189086914 ]
[.00498850345611572265 .02770539522171020507 .06733170747756958007 .23922916650772094726 .43367519378662109375 .69957635402679443359 1.22173422574996948242 1.89197597503662109375 ]
[.04550466537475585937 .19628169536590576171 .43975292444229125976 1.72467272281646728515 3.55145285129547119140 5.26081917285919189453 10.22290025949478149414 14.70899119377136230468 ]
];

%BTest Extra I Single proc
btes=[[.00000205039978027343 ]
[.00006272792816162109 ]
[.00049569606781005859 ]
[.00460376739501953125 ]
];

%AsBTest I
abt1=[[.00017890930175781250 .00061061382293701171 .00171971321105957031 .01491110324859619140 .03048996925354003906 .05403796434402465820 .09074841737747192382 .13222062587738037109 ]
[.00178928375244140624 .00765863656997680663 .02681936025619506835 .24103199243545532226 .45069755315780639648 .74304089546203613281 1.33269553184509277343 1.79059056043624877929 ]
[.00731787681579589843 .03146992921829223632 .10903337001800537109 .98664944171905517578 1.71361237764358520507 2.55139133930206298828 4.79149146080017089843 6.84182655811309814453 ]
[.03328713178634643554 .14571672677993774414 .46987895965576171875 3.40241512060165405273 7.32628574371337890625 11.15710138082504272460 19.61374927759170532226 28.19670959711074829101 ]
];

%AsBTest II
abt2=[[.00014578104019165038 .00065186023712158203 .00181452035903930664 .01514441967010498046 .02989934682846069336 .05510647296905517578 .09789230823516845703 .12522777318954467773 ]
[.00167057514190673828 .00782860517501831054 .02788406610488891601 .24576983451843261718 .48938393592834472656 .81499315500259399414 1.39569194316864013671 1.89093151092529296875 ]
[.00686690807342529296 .03193696737289428710 .11281089782714843749 1.01188426017761230468 1.77322223186492919921 2.65236514806747436523 5.59127113819122314453 7.48630504608154296875 ]
[.03070365190505981445 .14212100505828857421 .61466757059097290039 4.47570726871490478515 8.00906084775924682617 11.50245854854583740234 22.95568580627441406249 31.74515589475631713867 ]
];

%AsBTest I Single proc
abts=[[.00000123977661132812 ]
[.00001834630966186523 ]
[.00012542009353637695 ]
[.00097332000732421874 ]
];

%AsBTest Extra I
abte1=[[.00009121894836425781 .00051093101501464843 .00159475803375244140 .01428334712982177734 .03031673431396484375 .05428599119186401367 .08567875623703002929 .12497470378875732421 ]
[.00141597986221313476 .00752745866775512695 .02801098823547363281 .23134157657623291015 .45956765413284301757 .68400880098342895507 1.21100182533264160156 1.74233980178833007812 ]
[.00637850761413574218 .03422344923019409179 .11985471248626708984 .84686661958694458007 1.72265570163726806640 2.66890285015106201171 4.86015809774398803710 7.23635752201080322265 ]
[.03299909830093383789 .17550610303878784179 .58636940717697143554 3.11362709999084472656 7.61127105951309204101 13.21024830341339111328 22.25124372243881225585 32.79793027639389038085 ]
];

%AsBTest Extra II
abte2=[[.00010368824005126953 .00051406621932983398 .00159423351287841796 .01433484554290771484 .02856780290603637695 .05314098596572875976 .08460317850112915039 .12760519981384277343 ]
[.00141624212265014648 .00750633478164672851 .03498690128326416015 .24463042020797729492 .43069466352462768554 .76052079200744628906 1.24807882308959960937 1.72457746267318725585 ]
[.00635857582092285156 .03438888788223266601 .12034746408462524414 .85905413627624511718 1.74638656377792358398 2.65691615343093872070 4.82413984537124633789 7.08339384794235229492 ]
[.03311719894409179687 .17489801645278930663 .65335307121276855468 3.11248557567596435546 7.40736206769943237304 13.18765662908554077148 22.24258645772933959961 33.00158334970474243164 ]
];

%AsBTest Extra I Single proc
abtes=[[.00000199079513549804 ]
[.00006067752838134765 ]
[.00047758817672729492 ]
[.00436823368072509765 ]
];
