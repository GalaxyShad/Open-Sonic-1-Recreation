#pragma once

#include "core/Screen.h"

const Frame framesGhzGim[] = {
    Frame(irect(0,0,48,32)),  //Stone
    Frame(irect(48,0,96,88), v2i(48, 56)), //Slp platform
    Frame(irect(0,32,16,16)), //Bridge
    Frame(irect(0,90,64,30)), //Platform

    Frame(irect(144,0,16,64)), //Wall left shadow
    Frame(irect(160,0,16,64)), //Wall left
    Frame(irect(144,64,16,64)), //Wall right

    Frame(irect(64,88,32, 16)), //Bridge column
};

const Frame framesObjects[] = {
    //Animation sIdle 0 - 3
    Frame(irect(0,0,29,39),  v2i(14, 19)),
    Frame(irect(29,1,30,38), v2i(12,18)),
    Frame(irect(59,1,30,38), v2i(12,18)),
    Frame(irect(89,1,29,38), v2i(12,18)),
    //Animation,sWalk 4 - 9
    Frame(irect(147,2,24,37), v2i(13,17)),
    Frame(irect(171,2,36,37), v2i(19,19)),
    Frame(irect(207,1,29,38), v2i(18,18)),
    Frame(irect(236,1,27,37), v2i(14,17)),
    Frame(irect(263,1,37,37), v2i(18,19)),
    Frame(irect(300,0,32,38), v2i(14,18)),
    //Animation,sSit 10
    Frame(irect(118,13,29,26),v2i(9,6)),
    //Animation,sRun 11 - 14
    Frame(irect(332,1,31,35), v2i(20,15)),
    Frame(irect(363,0,31,36), v2i(20,16)),
    Frame(irect(394,1,30,36), v2i(19,15)),
    Frame(irect(424,0,31,36), v2i(20,16)),
    //Animation,sRoll 15 - 22
    Frame(irect(456,1,30,30), v2i(15,15)),
    Frame(irect(488,1,29,30), v2i(15,16)),
    Frame(irect(456,1,30,30), v2i(15,15)),
    Frame(irect(520,1,30,29), v2i(16,16)),
    Frame(irect(456,1,30,30), v2i(15,15)),
    Frame(irect(553,1,29,30), v2i(15,16)),
    Frame(irect(456,1,30,30), v2i(15,15)),
    Frame(irect(584,2,30,29), v2i(16,15)),
    //Animation,sSkid 23 - 25
    Frame(irect(615,0,30,35), v2i(12,15)),
    Frame(irect(645,0,31,36), v2i(13,16)),
    Frame(irect(681,37,33,36), v2i(15,16)),
    //Animation,sDiaWalk 26 - 31
    Frame(irect(680,0,34,37), v2i(17,18)),
    Frame(irect(718,0,36,43), v2i(18,19)),
    Frame(irect(758,0,35,40), v2i(17,18)),
    Frame(irect(795,2,38,38), v2i(18,16)),
    Frame(irect(835,2,38,43), v2i(19,17)),
    Frame(irect(875,2,41,34), v2i(18,16)),
    //Animation,sDiaRun 32 - 35
    Frame(irect(919,1,37,39), v2i(19,17)),
    Frame(irect(959,0,36,40), v2i(18,18)),
    Frame(irect(2,40,36,39), v2i(19,17)),
    Frame(irect(43,39,36,40), v2i(18,18)),
    //Animation,sHurt 36
    Frame(irect(80,39,40,28), v2i(20,14)),
    //Animation,sDie 37
    Frame(irect(120,39,34,43), v2i(17,21)),
    //Animation,sPush 38 - 41
    Frame(irect(154,40,29,36), v2i(14,17)),
    Frame(irect(183,39,24,37), v2i(9,18)),
    Frame(irect(207,40,28,36), v2i(13,17)),
    Frame(irect(235,39,24,37), v2i(9,18)),
    //Animation,sLookUp 42
    Frame(irect(259,40,28,38), v2i(14,18)),
    //Animation,sDash 43 - 46
    Frame(irect(287,40,39,31), v2i(28,11)),
    Frame(irect(327,39,39,32), v2i(28,12)),
    Frame(irect(367,37,39,31), v2i(28,11)),
    Frame(irect(408,36,38,32), v2i(27,12)),
    //Animation,sDiaDash 47 - 50
    Frame(irect(449,37,37,41), v2i(18,14)),
    Frame(irect(489,32,34,40), v2i(18,14)),
    Frame(irect(530,32,37,43), v2i(18,14)),
    Frame(irect(569,33,35,39), v2i(18,14)),
    //Animation,sSpring 51 - 53
    Frame(irect(607,36,25,43), v2i(13,22)),
    Frame(irect(632,36,20,44), v2i(8,22)),
    Frame(irect(652,37,29,44), v2i(14,22)),
    //Animation,sFall 54 - 55
    Frame(irect(714,43,36,48), v2i(18,24)),
    Frame(irect(750,43,36,47), v2i(18,23)),
    //Animation,Monitor 56 - 57
    Frame(irect(786,40,30,30), v2i(15,15)),
    Frame(irect(816,45,32,16), v2i(16,1)),
    //Animation,MonitorIcon 58 - 64
    Frame(irect(786,70,16,14), v2i(8,7)),
    Frame(irect(802,70,16,14), v2i(8,7)),
    Frame(irect(818,61,16,14), v2i(8,7)),
    Frame(irect(834,61,16,14), v2i(8,7)),
    Frame(irect(850,61,16,14), v2i(8,7)),
    Frame(irect(818,75,16,14), v2i(8,7)),
    Frame(irect(834,75,16,14), v2i(8,7)),
    //Animation,Springs 65 - 68
    Frame(irect(848,45,28,16), v2i(14,8)),
    Frame(irect(866,61,28,32), v2i(14,24)),
    Frame(irect(876,37,28,16), v2i(14,8)),
    Frame(irect(894,61,28,32), v2i(14,24)),
    //Animation,KillScore 69 - 74
    Frame(irect(876,53,9,8), v2i(4,4)),
    Frame(irect(885,53,14,8), v2i(7,4)),
    Frame(irect(904,37,15,8), v2i(7,4)),
    Frame(irect(918,53,15,8), v2i(7,4)),
    Frame(irect(899,53,19,8), v2i(9,4)),
    Frame(irect(904,45,24,8), v2i(12,4)),
    //Animation,Act 75 - 79
    Frame(irect(928,40,24,8), v2i(12,4)),
    Frame(irect(968,40,16,16), v2i(8,8)),
    Frame(irect(933,48,7,24), v2i(3,12)),
    Frame(irect(940,48,14,24), v2i(7,12)),
    Frame(irect(954,48,14,24), v2i(7,12)),
    //Animation,Ring 80 - 87
    Frame(irect(984,40,16,16), v2i(8,8)),
    Frame(irect(968,56,12,16), v2i(6,8)),
    Frame(irect(980,56,6,16), v2i(3,8)),
    Frame(irect(986,56,12,16), v2i(6,8)),
    Frame(irect(935,72,16,14), v2i(8,7)),
    Frame(irect(951,72,16,14), v2i(8,7)),
    Frame(irect(967,72,16,14), v2i(8,7)),
    Frame(irect(983,72,16,14), v2i(8,7)),
    //Animation,SkidDust 88 - 91
    Frame(irect(1000,32,13,14), v2i(6,7)),
    Frame(irect(1000,46,11,12), v2i(5,6)),
    Frame(irect(1001,58,11,9), v2i(5,4)),
    Frame(irect(1002,67,8,10), v2i(4,5)),
    //Animation,Flicky 92 - 94
    Frame(irect(850,75,11,20), v2i(5,10)),
    Frame(irect(786,84,17,15), v2i(8,7)),
    Frame(irect(803,85,15,14), v2i(7,7)),
    //Animation,Explosion 95 - 99
    Frame(irect(1166,0,20,16), v2i(10,8)),
    Frame(irect(1165,16,26,30), v2i(13,15)),
    Frame(irect(1168,46,33,32), v2i(16,16)),
    Frame(irect(1191,0,38,38), v2i(19,19)),
    Frame(irect(1202,38,36,38),v2i(18,19)),
    //Animation,Invisible 100 - 103
    Frame(irect(1238,1,48,47), v2i(24,23)),
    Frame(irect(1286,0,48,47), v2i(24,23)),
    Frame(irect(1241,48,46,460), v2i(23,23)),
    Frame(irect(1288,48,46,46), v2i(23,23)),
    //Animation,SignPost 104 - 109
    Frame(irect(1406,64,6,16),  v2i(3,8)),
    Frame(irect(1390,0,48,32),  v2i(24,16)),
    Frame(irect(1334,56,32,32), v2i(16,16)),
    Frame(irect(1398,64,8,32),  v2i(4,16)),
    Frame(irect(1366,64,32,32), v2i(16,16)),
    Frame(irect(1390,32,48,32), v2i(24,16)),
    //Animation,ActTitleThing 110
    Frame(irect(1334,0,56,56), v2i(28,28)),
    //Animation,Chopper 111 - 112
    Frame(irect(1090,0,30,32), v2i(15,16)),
    Frame(irect(1088,62,29,31), v2i(14,16)),
    //Animation,Crabmeat 113 - 116
    Frame(irect(1120,0,42,31), v2i(21,15)),
    Frame(irect(1120,31,44,31), v2i(22,15)),
    Frame(irect(1120,0,42,31), v2i(21,15)),
    Frame(irect(1120,62,48,30), v2i(24,14)),
    //Animation,buzzBomber 117 - 124
    Frame(irect(670,81,44,19), v2i(22,9)),
    Frame(irect(1046,64,36,29), v2i(18,14)),
    Frame(irect(714,91,35,8), v2i(17,4)),
    Frame(irect(749,91,37,6), v2i(18,3)),
    Frame(irect(681,76,6,5), v2i(3,2)),
    Frame(irect(687,76,10,5), v2i(5,2)),
    Frame(irect(640,82,16,16), v2i(8,8)),
    Frame(irect(656,82,14,14), v2i(7,7)),
    //Animation,bullet 125 - 127
    Frame(irect(1010,65,12,12), v2i(6,6)),
    Frame(irect(1022,65,12,12), v2i(6,6)),
    Frame(irect(1034,65,12,12), v2i(6,6)),
    //Animation,Debug 128 - 133
    Frame(irect(0,84,16,16), v2i(8,8)),
    Frame(irect(16,84,16,16), v2i(8,8)),
    Frame(irect(32,84,16,16), v2i(8,8)),
    Frame(irect(48,84,16,16), v2i(8,8)),
    Frame(irect(64,84,16,16), v2i(8,8)),
    Frame(irect(80,84,16,16), v2i(8,8)),
    // Anim Motobug 134 - 137
    Frame(irect(1012, 0, 39, 29), v2i(19, 14)),
    Frame(irect(1051, 1, 39, 28), v2i(19, 13)),
    Frame(irect(1013, 29, 40, 29), v2i(20, 14)),
    Frame(irect(1053, 32, 40, 28), v2i(20, 13)),
    // Spike 138
    Frame(irect(1001, 0, 8, 32)),
};